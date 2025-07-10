/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:20:42 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/10 16:14:43 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Entry point for the minishell program.
 *
 * This function sets up the shell environment, handles the main input loop,
 * reads user input, parses the command, and dispatches it for execution.
 *
 * @param argc Number of command-line arguments (unused).
 * @param argv Array of command-line argument strings (unused).
 * @param envp Array of environment variable strings.
 * @return int Exit status of the last executed command.
 */
int main(int argc, char **argv, char **envp)
{
	char *line; // Input line from user
	char **args = NULL; // Parsed command arguments
	int i;
	int status = 0; // Exit status of last command
	t_shell shell; // Custom struct holding shell state

	(void)argc; // Marking unused parameters to avoid warnings
	(void)argv;
	setup_signals(); // Setup handlers for signals like Ctrl+C
	//disable_ctrl_echo(); // Disable echoing of ^C or ^\ in terminal
	shell.envp = copy_env(envp); // Create a copy of the environment
	shell.last_exit_status = 0; // Initialize exit status
	while (1)
	{
		line = read_prompt(); // Display prompt and get user input
		if (!line) // NULL line means Ctrl+D (EOF)
			break;
		if (*line) // Add non-empty lines to history
			add_history(line);
		args = parse_input(line);  // Tokenize the input string
		if (args && args[0]) // If we have a valid command
			status = handle_command(&shell, args); // Dispatch command
		free(line);
		if (args) // Free argument array
		{
			i = 0;
			while (args[i])
				free(args[i++]);
			free(args);
		}
	}
	// Cleanup environment variables
	if (shell.envp)
	{
		i = 0;
		while (shell.envp[i])
			free(shell.envp[i++]);
		free(shell.envp);
	}
	return status; // Return last command's status
}


