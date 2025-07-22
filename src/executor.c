/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:13:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/22 19:12:23 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redirections(t_cmd *command)
{
	int	in_fd;
	int	out_fd;

	in_fd = -1;
	out_fd = -1;
	if (command->infile && command->infile_name)
	{
		in_fd = open(command->infile_name, O_RDONLY);
		if (in_fd < 0)
		{
			perror(command->infile_name);
			exit (1);
		}
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (command->outfile && command->outfile_name)
	{
		// printf(COLOR_R"outfile: %s\n"COLOR_X, command->outfile_name);
		if (command->append)
			out_fd = open(command->outfile_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			out_fd = open(command->outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd < 0)
		{
			perror(command->outfile_name);
			exit (1);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
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
int execute_command(t_cmd *command, t_shell *shell, char **args)
{
	pid_t pid; // Process ID returned by fork()
	int status; // Used to store child's exit status
	char *path; // Full path to executable file

	// printf("execute_command: %s\n", args[0]); // Debug output
	// Check if the command is directly executable
	if (!args || !args[0])
		return 1;
	path = NULL;
	if (!command->isbuiltin)
	{
		if (access(args[0], X_OK) == 0)
			path = ft_gcstrdup(CAT_ARGS, args[0]); // Command has absolute/relative path
		else
			path = find_executable(shell, args[0]); // Try to find it in $PATH
		if (!path)
		{
			ft_print_error("minishell: command not found: ");
			ft_print_error(args[0]);
			ft_print_error("\n");
			return 127; // Standard "command not found" return code
		}
	}
	pid = fork(); // Create a child process
	if (pid == -1)
	{
		perror("fork");
		if (path)
			free(path);
		return 1;
	}
	else if (pid == 0)
	{
		setup_redirections(command);
		if (!command->isbuiltin)
		{
			execve(path, args, shell->envp);
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
			exit(0);// Exit child process after running built-in
		}
		exit(1);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (path)
			free(path);
		return WEXITSTATUS(status);
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
static int handle_command(t_cmd *command, char **args)
{
	t_shell *shell;

	shell = get_shell(); // Get the current shell state
	if (!args || !args[0])
		return 0;
	if (!ft_strcmp(args[0], "cd"))
		return built_cd(args);
	else if (!ft_strcmp(args[0], "export"))
		return built_export(shell, args);
	else if (!ft_strcmp(args[0], "unset"))
		return built_unset(shell, args);
	else if (!ft_strcmp(args[0], "exit"))
		built_exit();
	else if (ft_strcmp(args[0], "env") == 0
		|| ft_strcmp(args[0], "echo") == 0 || ft_strcmp(args[0], "pwd") == 0)
		command->isbuiltin = 1;
	return (execute_command(command, shell, args));
}

static void run_child_process(int fd, int p_fd[2], t_cmd *command, int dup_fd)
{
	int status;

	// setup_redirections(command); // Setup input/output redirections if needed
	dup2(fd, dup_fd);
	close(p_fd[0]);
	close(p_fd[1]);
	status = ft_run_commands(command);
	free_gc();
	exit(status);
}

static int	handle_pipe(t_cmd *command_1, t_cmd *command_2)
{
	int p_fd[2];
	pid_t pid_a;
	pid_t pid_b;
	int status_a;
	int status_b;

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

int	ft_run_commands(t_cmd *com)
{
	int status;

	//printf("run %s\n", com->token->value);
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
