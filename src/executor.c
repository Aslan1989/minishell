/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:13:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/24 13:46:54 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the execution of a command.
 *
 * This function checks if the command is a built-in that doesn't require
 * special handling (like redirections or pipes). If it is, it calls the
 * appropriate built-in function. It calls `execute_command` for
 * external commands or built-ins that require execution.
 *
 * @param command Pointer to the command structure containing command details.
 * @param args Array of arguments, where args[0] is the command name.
 * @return int Exit status of the executed command.
 */
static int	handle_command(t_cmd *command, char **args)
{
	t_shell	*shell;

	shell = get_shell();
	if (!args || !args[0])
		return (0);
	if (!ft_strcmp(args[0], "cd"))
		return (built_cd(args));
	else if (!ft_strcmp(args[0], "export"))
		return (built_export(shell, args));
	else if (!ft_strcmp(args[0], "unset"))
		return (built_unset(shell, args));
	else if (!ft_strcmp(args[0], "exit"))
		built_exit();
	else if (ft_strcmp(args[0], "env") == 0
		|| ft_strcmp(args[0], "echo") == 0 || ft_strcmp(args[0], "pwd") == 0)
		command->isbuiltin = 1;
	return (execute_command(command, shell, args));
}

/**
 * @brief Runs a child process with the specified file descriptors.
 *
 * This function sets up the necessary file descriptors for the child process
 * and then executes the command.
 *
 * @param fd File descriptor to duplicate.
 * @param p_fd Pipe file descriptors.
 * @param command Pointer to the command structure.
 * @param dup_fd File descriptor to duplicate to (e.g., STDOUT_FILENO).
 */
static void	run_child_process(int fd, int p_fd[2], t_cmd *command, int dup_fd)
{
	int	status;

	dup2(fd, dup_fd);
	close(p_fd[0]);
	close(p_fd[1]);
	status = ft_run_commands(command);
	free_gc();
	exit(status);
}

/**
 * @brief Handles the execution of a command with pipes.
 *
 * This function creates a pipe, forks two child processes, and sets up
 * the necessary file descriptors for each child to communicate through the pipe.
 *
 * @param command_1 First command structure to execute.
 * @param command_2 Second command structure to execute.
 * @return int Exit status of the second command.
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
	pid_a = fork();
	if (pid_a < 0)
		return (perror("fork"), close(p_fd[0]), close(p_fd[1]), 1);
	if (pid_a == 0)
		run_child_process(p_fd[1], p_fd, command_1, STDOUT_FILENO);
	pid_b = fork();
	if (pid_b < 0)
		return (perror("fork"), close(p_fd[0]), close(p_fd[1]), 1);
	if (pid_b == 0)
		run_child_process(p_fd[0], p_fd, command_2, STDIN_FILENO);
	close(p_fd[1]);
	close(p_fd[0]);
	waitpid(pid_a, &status_a, 0);
	waitpid(pid_b, &status_b, 0);
	return (WEXITSTATUS(status_b));
}

/**
 * @brief Runs a list of commands, handling pipes and built-ins.
 *
 * This function iterates through the command list, executing each command
 * in the appropriate way (built-in or external). It handles pipes between
 * commands and sets up redirections as needed.
 *
 * @param com Pointer to the first command in the list.
 * @return int Exit status of the last executed command.
 */
int	ft_run_commands(t_cmd *com)
{
	int	status;

	if (!com)
		return (0);
	if (com->type == TOK_WORD)
		return (handle_command(com, com->commands));
	if (com->type == TOK_PIPE)
		return (handle_pipe(com->next_a, com->next_b));
	if (com->type == TOK_AND)
	{
		status = ft_run_commands(com->next_a);
		if (status == 0)
			return (ft_run_commands(com->next_b));
		return (status);
	}
	if (com->type == TOK_OR)
	{
		status = ft_run_commands(com->next_a);
		if (status != 0)
			return (ft_run_commands(com->next_b));
		return (status);
	}
	return (0);
}
