/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:13:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/31 13:45:16 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Resolve command path if not a builtin; set 127 on "not found".
 *
 * Logic:
 *  - If it's not a builtin:
 *      * If args[0] is directly executable (access X_OK), use it as path.
 *      * Else search in $PATH via find_executable().
 *      * If nothing found, print "command not found" and return 127.
 *  - If it is a builtin, nothing to resolve here → return 0.
 *
 * External command? resolve path
 * Provided string is an executable path
 * Otherwise try $PATH lookup
 * @param command Command node (holds .isbuiltin and .path).
 * @param shell   Shell state (envp used by find_executable()).
 * @param args    argv for the command; args[0] is the name.
 * @return int 0 on success (or builtin), 127 if command not found.
 */
int	find_command(t_cmd *command, t_shell *shell, char **args)
{
	if (!command->isbuiltin)
	{
		if (access(args[0], X_OK) == 0)
			command->path = ft_gcstrdup(CAT_CMD, args[0]);
		else
			command->path = find_executable(shell, args[0]);
		if (!command->path)
		{
			ft_print_error("minishell: ");
			ft_print_error(args[0]);
			ft_print_error(": command not found\n");
			return (127);
		}
	}
	return (0);
}

/**
 * @brief Validate target path and exec an external program, exiting on error.
 *
 * Checks:
 *  - Existence (F_OK) → if missing: exit(127).
 *  - Not a directory → if directory: exit(126).
 *  - Executable (X_OK) → if permission denied: exit(126).
 * Then execve(). If it returns, handle ENOENT (127) or generic error (126).
 *
 * Does file exist?
 * POSIX: not found
 * Is it a directory?
 * Found but not runnable
 * Lacks execute permission?
 * Replace image
 * e.g., script with missing interpreter
 * Other execution errors
 * @param command Command node with resolved .path.
 * @param shell   Shell (to pass shell->envp to execve()).
 * @param args    argv for execve().
 */
static void	valid_and_exec_extern(t_cmd *command, t_shell *shell, char **args)
{
	struct stat	st;

	if (access(command->path, F_OK) != 0)
	{
		print_arg_err(args[0], "No such file or directory");
		exit(127);
	}
	if (stat(command->path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_arg_err(args[0], "is a directory");
		exit(126);
	}
	if (access(command->path, X_OK) != 0)
	{
		print_arg_err(args[0], "Permission denied");
		exit(126);
	}
	execve(command->path, args, shell->envp);
	if (errno == ENOENT)
	{
		print_arg_err(args[0], "No such file or directory");
		exit(127);
	}
	perror("minishell");
	exit(126);
}

/**
 * @brief Execute builtin commands in a child process (subset: echo, pwd, env).
 *
 * Note: Builtins that must persist state (cd, export, unset, exit) are usually
 * handled in the parent. This function runs "safe-in-child" builtins only.
 *
 * @param shell Shell state (for env).
 * @param args  argv (args[0] is the builtin name).
 * @return int  Exit status of the builtin (0 on success, 1 if unknown).
 */
static int	execute_builtin_child(t_shell *shell, char **args)
{
	int	status;

	status = 0;
	if (!ft_strcmp(args[0], "echo"))
		status = built_echo(args);
	else if (!ft_strcmp(args[0], "pwd"))
		status = built_pwd();
	else if (!ft_strcmp(args[0], "env"))
		status = built_env(shell, args);
	else
		status = 1;
	return (status);
}

/**
 * @brief Child-side execution path: setup redirs, run external or builtin,
 * then exit.
 *
 * Steps:
 *  - Restore default signals in interactive mode.
 *  - Apply I/O redirections for this command.
 *  - If no resolved path → print "not found" and exit 127.
 *  - If external → validate and exec (never returns on success).
 *  - If builtin → run, free GC, clear history, and exit with its status.
 *
 * Child should receive default signal behavior
 * Apply redirections (dup2, close, etc.)
 * Shouldn’t happen if find_command succeeded,
 * but keep a defensive check
 * External program
 * Child-safe builtin
 * Cleanup before leaving the child
 * @param command Command node (path, redirections, isbuiltin).
 * @param shell   Shell state (envp).
 * @param args    argv for command/builtin.
 */
static void	execute_child_command(t_cmd *command, t_shell *shell, char **args)
{
	int			status;

	if (get_shell()->is_interactive)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	setup_redirections(command);
	if (!command->isbuiltin)
		valid_and_exec_extern(command, shell, args);
	else
	{
		status = execute_builtin_child(shell, args);
		free_gc();
		clear_history();
		exit(status);
	}
}

/**
 * @brief Execute a single (non-pipeline) command by forking and waiting.
 *
 * Flow:
 *  - Validate argv. Empty name → 127.
 *  - Resolve path unless builtin; error → 127.
 *  - fork():
 *      * child: execute_child_command() → exit
 *      * parent: waitpid(); map result to shell exit codes.
 *        If terminated by SIGQUIT, print "Quit: 3" and return 128+signal.
 *
 * Resolve path or report 127
 * Create a child
 * Child branch
 * Parent branch
 * Wait for child to finish
 * Normal exit → propagate code
 * @param command Command node with redirs/isbuiltin fields.
 * @param shell   Shell state (envp).
 * @param args    argv for this command.
 * @return int Exit status compatible with POSIX shells.
 */
int	execute_command(t_cmd *command, t_shell *shell, char **args)
{
	pid_t	pid;
	int		status;
	int		rc;

	rc = precheck_command(command, shell, args);
	if (rc != 0)
		return (rc);
	parent_signals_exec_begin();
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		execute_child_command(command, shell, args);
	waitpid(pid, &status, 0);
	parent_signals_exec_end();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (1);
}
