/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:18:18 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/08 18:18:18 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Dispatch a builtin command (decide which one to call).
 *
 * @param cmd  Command node (not always used).
 * @param sh   Shell state.
 * @param args Argument vector (args[0] = command name).
 * @return int Exit status of builtin, or 1 if not recognized.
 */
static int	dispatch_builtin(t_cmd *cmd, t_shell *sh, char **args)
{
	if (!ft_strcmp(args[0], "cd"))
		return (built_cd(args));
	if (!ft_strcmp(args[0], "export"))
		return (built_export(sh, args));
	if (!ft_strcmp(args[0], "unset"))
		return (built_unset(sh, args));
	if (!ft_strcmp(args[0], "exit"))
		built_exit(args);
	if (!ft_strcmp(args[0], "echo"))
		return (built_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (built_pwd());
	if (!ft_strcmp(args[0], "env"))
		return (built_env(sh, args));
	(void)cmd;
	return (1);
}

/**
 * @brief Run a builtin command in the parent process (not in a fork).
 *
 * This is needed so that changes in the parent state (env, pwd, exit) persist.
 * Also applies redirections if needed, then restores stdio.
 * Flag if we need to restore stdio after redirs
 * If builtin has redirections
 * Execute builtin directly
 * Restore original stdin/out
 *
 * @param cmd   Command node (may have redirections).
 * @param sh    Shell state.
 * @param args  Argument vector.
 * @return int  Exit status of the builtin.
 */
static int	run_builtin_parent(t_cmd *cmd, t_shell *sh, char **args)
{
	int	save_in;
	int	save_out;
	int	status;
	int	need_restore;

	save_in = -1;
	save_out = -1;
	need_restore = 0;
	if (cmd->redir)
	{
		if (parent_redirs_apply(cmd, &save_in, &save_out) != 0)
			return (1);
		need_restore = 1;
	}
	status = dispatch_builtin(cmd, sh, args);
	if (need_restore)
		parent_stdio_restore(&save_in, &save_out);
	return (status);
}

/**
 * @brief Handle a single command node.
 *
 * - If builtin → run in parent.
 * - Else → run external command via fork+exec.
 * Builtins run in parent
 * External commands → fork/exec
 * Update global last exit status
 *
 * @param command Command node (must be TOK_WORD).
 * @param args    Argument vector.
 * @return int    Exit status.
 */
static int	handle_command(t_cmd *command, char **args)
{
	t_shell	*shell;
	int		status;

	if (!command || !args || !args[0])
		return (1);
	shell = get_shell();
	if (!shell)
		return (1);
	if (is_builtin(args[0]))
		status = run_builtin_parent(command, shell, args);
	else
		status = execute_command(command, shell, args);
	shell->last_exit_status = status;
	return (status);
}

/**
 * @brief Handle a pipe node: cmd1 | cmd2
 *
 * - Create a pipe.
 * - Fork left child: redirect STDOUT → pipe write.
 * - Fork right child: redirect STDIN ← pipe read.
 * - Wait for both children.
 * Left child writes
 *
 * @param command_1 Left command node.
 * @param command_2 Right command node.
 * @return int Exit status of right command (like bash).
 */
static int	handle_pipe(t_cmd *command_1, t_cmd *command_2)
{
	int		p_fd[2];
	pid_t	pid_a;
	pid_t	pid_b;
	int		status_a;
	int		status_b;

	if (pipe(p_fd) < 0)
		return (perror("pipe"), 1);
	pid_a = fork_pipe_child(command_1, p_fd[1], p_fd, STDOUT_FILENO);
	if (pid_a < 0)
		return (perror("fork"), close(p_fd[0]), close(p_fd[1]), 1);
	pid_b = fork_pipe_child(command_2, p_fd[0], p_fd, STDIN_FILENO);
	if (pid_b < 0)
	{
		kill(pid_a, SIGKILL);
		return (perror("fork"), close(p_fd[0]), close(p_fd[1]), 1);
	}
	close(p_fd[0]);
	close(p_fd[1]);
	waitpid(pid_a, &status_a, 0);
	waitpid(pid_b, &status_b, 0);
	return (get_exit_status(status_b));
}

/**
 * @brief Main entry: run a command AST.
 *
 * Dispatches based on node type:
 *  - TOK_WORD  → single command.
 *  - TOK_PIPE  → command1 | command2.
 *  - TOK_AND   → command1 && command2.
 *  - TOK_OR    → command1 || command2.
 *
 * Also resets syntax_error and returns code 258 if it was set.
 *
 * @param com AST command node.
 * @return int Exit status of last executed command.
 */
int	ft_run_commands(t_cmd *com)
{
	int	status;

	if (get_shell()->syntax_error)
		return (get_shell()->syntax_error = 0, 258);
	if (get_shell()->heredoc_interrupted)
		return (get_shell()->heredoc_interrupted = 0, 130);
	if (!com)
		return (0);
	if (com->type == TOK_WORD)
	{
		status = handle_command(com, com->commands);
		get_shell()->last_exit_status = status;
		return (status);
	}
	if (com->type == TOK_PIPE)
	{
		status = handle_pipe(com->next_a, com->next_b);
		get_shell()->last_exit_status = status;
		return (status);
	}
	if (com->type == TOK_AND)
		return (run_and_node(com));
	if (com->type == TOK_OR)
		return (run_or_node(com));
	return (0);
}
