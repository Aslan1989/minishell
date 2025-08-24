/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:10:20 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/24 15:24:21 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Run a child process in a pipeline or standalone execution.
 *
 * Steps:
 *  - Reset signals to default (only if interactive shell).
 *  - Duplicate one file descriptor onto STDIN or STDOUT.
 *  - Close unused pipe ends.
 *  - Execute the command node.
 *  - Exit with the returned status.
 *
 * @param fd     File descriptor to duplicate (read or write end of pipe).
 * @param p_fd   Array of 2 pipe file descriptors [read, write].
 * @param command Command node to execute.
 * @param dup_fd Target (STDIN_FILENO or STDOUT_FILENO).
 */
static void	run_child_process(int fd, int p_fd[2], t_cmd *command, int dup_fd)
{
	int	status;

	if (get_shell()->is_interactive)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (dup2(fd, dup_fd) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
	close(p_fd[0]);
	close(p_fd[1]);
	status = ft_run_commands(command);
	free_gc();
	exit(status);
}

/**
 * @brief Fork a new child process for a command in a pipeline.
 *
 * @param cmd     Command node to execute.
 * @param fd      File descriptor (pipe end to connect).
 * @param p_fd    Pipe file descriptors [read, write].
 * @param dup_fd  Target to redirect fd (STDIN or STDOUT).
 * @return pid_t  Child process ID, or < 0 on fork failure.
 */
pid_t	fork_pipe_child(t_cmd *cmd, int fd, int p_fd[2], int dup_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		run_child_process(fd, p_fd, cmd, dup_fd);
	return (pid);
}

/**
 * @brief Execute an AND (&&) node: run B only if A succeeded (exit code 0).
 *
 * @param node Command node of type TOK_AND.
 * @return int Exit status of the last executed command.
 */
int	run_and_node(t_cmd *node)
{
	int	status;

	status = ft_run_commands(node->next_a);
	get_shell()->last_exit_status = status;
	if (status == 0)
		status = ft_run_commands(node->next_b);
	get_shell()->last_exit_status = status;
	return (status);
}

int	run_or_node(t_cmd *node)
{
	int	status;

	status = ft_run_commands(node->next_a);
	get_shell()->last_exit_status = status;
	if (status != 0)
		status = ft_run_commands(node->next_b);
	get_shell()->last_exit_status = status;
	return (status);
}
