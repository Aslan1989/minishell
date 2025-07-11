/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:13:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/11 13:11:38 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
int execute_command(t_shell *shell, char **args)
{
	pid_t pid; // Process ID returned by fork()
	int status; // Used to store child's exit status
	char *path; // Full path to executable file

	// Check if the command is directly executable
	if (!args || !args[0])
		return 1;
	if (access(args[0], X_OK) == 0)
		path = ft_strdup(args[0]); // Command has absolute/relative path
	else
		path = find_executable(shell, args[0]); // Try to find it in $PATH
	if (!path)
	{
		write(2, "minishell: command not found: ", 30);
		write(2, args[0], ft_strlen(args[0]));
		write(2, "\n", 1);
		return 127; // Standard "command not found" return code
	}
	pid = fork(); // Create a child process
	if (pid == -1)
	{
		perror("fork");
		free(path);
		return 1;
	}
	else if (pid == 0)
	{
		// Child process: execute the command
		execve(path, args, shell->envp); // Use your own environment
		perror("execve failed");
		exit(1);
	}
	else
	{
		// Parent process: wait for child to finish
		//waitpid(pid, &status, 0);
		free(path);
		return WEXITSTATUS(status); // Return child's exit code
	}
	return (status);
}

/**
 * @brief Decides what to do with a parsed command: run a built-in or external command.
 *
 * This is the "brain" that figures out what kind of command the user typed.
 * If it's a known built-in like `cd` or `echo`, it runs the appropriate function.
 * If it's not a built-in, it tries to run it as an external command via execve.
 *
 * @param shell Pointer to shell state (environment, etc.).
 * @param args Array of strings where args[0] is command name and rest are arguments.
 * @return int Exit status of the executed command.
 */
int handle_command(t_shell *shell, char **args)
{
	if (!args || !args[0])
		return 0;
	if (!ft_strcmp(args[0], "echo"))
		return built_echo(args);
	else if (!ft_strcmp(args[0], "cd"))
		return built_cd(args);
	else if (!ft_strcmp(args[0], "pwd"))
		return built_pwd();
	else if (!ft_strcmp(args[0], "export"))
		return built_export(shell, args);
	else if (!ft_strcmp(args[0], "unset"))
		return built_unset(shell, args);
	else if (!ft_strcmp(args[0], "env"))
		return built_env(shell);
	else if (!ft_strcmp(args[0], "exit"))
		built_exit(args);
	// If it's not a built-in command, try executing it from disk
	return execute_command(shell, args);
}
