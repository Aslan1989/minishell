/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:10:20 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/09 15:22:23 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Forked child helper: duplicate one end of a pipe to STDIN/STDOUT.
 * @param fd The end we want to dup.
 * @param p_fd The pipe fds (both closed here).
 * @param command Sub-AST to execute in the child.
 * @param dup_fd Either STDIN_FILENO or STDOUT_FILENO.
 */
static void	run_child_process(int fd, int p_fd[2], t_cmd *command, int dup_fd)
{
	int	status;

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

pid_t	fork_pipe_child(t_cmd *cmd, int fd, int p_fd[2], int dup_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		run_child_process(fd, p_fd, cmd, dup_fd);
	return (pid);
}

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

int	run_semi_node(t_cmd *node)
{
	int	status;

	status = ft_run_commands(node->next_a);
	get_shell()->last_exit_status = status;
	status = ft_run_commands(node->next_b);
	get_shell()->last_exit_status = status;
	return (status);
}
