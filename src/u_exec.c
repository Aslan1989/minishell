/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_exec.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:13:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/24 14:02:21 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Finds the executable path for a command.
 *
 * This function checks if the command is a built-in or an external command.
 * If it's an external command, it searches for the executable in the
 * system's PATH.
 *
 * @param command Pointer to the command structure.
 * @param shell Pointer to the shell structure containing environment variables.
 * @param args Array of arguments, where args[0] is the command name.
 * @return int 0 if the command is found, 127 if not found.
 */
static int	find_command(t_cmd *command, t_shell *shell, char **args)
{
	if (!command->isbuiltin)
	{
		if (access(args[0], X_OK) == 0)
			command->path = ft_gcstrdup(CAT_ARGS, args[0]);
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

/**
 * @brief Executes a command in a child process.
 *
 * This function sets up the redirections for the command, then uses `execve`
 * to run the command in a child process. If the command is a built-in, it
 * calls the appropriate built-in function instead.
 *
 * @param command Pointer to the command structure containing command details.
 * @param shell Pointer to the shell structure containing
 * environment variables.
 * @param args Array of arguments, where args[0] is the command name.
 */
static void	execute_child_command(t_cmd *command, t_shell *shell, char **args)
{
	setup_redirections(command);
	if (!command->isbuiltin)
	{
		execve(command->path, args, shell->envp);
		perror("execve failed");
	}
	else
	{
		if (!ft_strcmp(args[0], "echo"))
			built_echo(args);
		else if (!ft_strcmp(args[0], "pwd"))
			built_pwd();
		else if (!ft_strcmp(args[0], "env"))
			built_env(shell);
		free_gc();
		clear_history();
		exit(0);
	}
	exit(1);
}

/**
 * @brief Executes an external command (not a built-in).
 *
 * This function uses `fork()` to create a child process, and then calls
 * `execve()` in the child to run the external program. The parent process
 * waits until the child process finishes.
 *
 * @param shell Pointer to shell structure with environment variables.
 * @param args Arguments, where args[0] is the program name, rest are arguments.
 * @return int The exit status returned by the child process.
 */
int	execute_command(t_cmd *command, t_shell *shell, char **args)
{
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (1);
	if (find_command(command, shell, args) != 0)
		return (127);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		if (command->path)
			free(command->path);
		return (1);
	}
	else if (pid == 0)
		execute_child_command(command, shell, args);
	else
	{
		waitpid(pid, &status, 0);
		if (command->path)
			free(command->path);
		return (WEXITSTATUS(status));
	}
	return (status);
}
