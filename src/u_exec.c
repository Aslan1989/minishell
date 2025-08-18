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
 * @brief Resolve command->path for external commands, producing 127 on error.
 * @param command Node where path is stored.
 * @param shell Shell state (for PATH/envp).
 * @param args argv (args[0] is the candidate).
 * @return int 0 on success, 127 on not found.
 */
static int	find_command(t_cmd *command, t_shell *shell, char **args)
{
	if (!command->isbuiltin)
	{
		if (access(args[0], X_OK) == 0)
			command->path = ft_gcstrdup(CAT_CMD, args[0]);
		else
			command->path = find_executable(shell, args[0]);
		if (!command->path)
		{
			ft_print_error("minishell: command not found: ");
			ft_print_error(args[0]);
			ft_print_error("\n");
			return (127);
		}
	}
	return (0);
}

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
 * @brief Code path for forked children: set up redirs and exec or run builtin.
 * @param command Node (contains redirs and resolved path for external).
 * @param shell Shell state (envp for execve).
 * @param args argv.
 */
static void	execute_child_command(t_cmd *command, t_shell *shell, char **args)
{
	int			status;

	setup_redirections(command);
	if (!command->path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putendl_fd(args[0], STDERR_FILENO);
		exit(127);
	}
	if (!command->isbuiltin)
		valid_and_exec_extern(command, shell, args);
	else
	{
		status = 0;
		if (!ft_strcmp(args[0], "echo"))
			status = built_echo(args);
		else if (!ft_strcmp(args[0], "pwd"))
			status = built_pwd();
		else if (!ft_strcmp(args[0], "env"))
			status = built_env(shell, args);
		free_gc();
		clear_history();
		exit(status);
	}
}

/**
 * @brief Execute external command (or builtin in a child for pipelines).
 * @param command Node with redirections/argv/path.
 * @param shell Shell state.
 * @param args argv.
 * @return int Collected status according to waitpid.
 */
int	execute_command(t_cmd *command, t_shell *shell, char **args)
{
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (1);
	if (args[0][0] == '\0')
	{
		print_arg_err(args[0], "minishell: command not found: ");
		ft_print_error("\n");
		return (127);
	}
	if (find_command(command, shell, args) != 0)
		return (127);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
		execute_child_command(command, shell, args);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		return (1);
	}
	return (0);
}
