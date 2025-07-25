/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:20:42 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/24 15:00:45 by psmolin          ###   ########.fr       */
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
	char	*line; // Input line from user
	char	**args; // Parsed command arguments
	int		status = 0; // Exit status of last command
	t_shell	*shell; // Custom struct holding shell state
	t_cmd	*comms;

	ft_print_banner();
	(void)argc; // Marking unused parameters to avoid warnings
	(void)argv;
	comms = NULL;
	shell = get_shell();
	args = NULL;
	(void)args;
	setup_signals(); // Setup handlers for signals like Ctrl+C
	disable_ctrl_echo(); // Disable echoing of ^C or ^\ in terminal
	shell->envp = copy_env(envp); // Create a copy of the environment
	shell->last_exit_status = 0; // Initialize exit status
	while (1)
	{
		line = read_prompt(); // Display prompt and get user input
		if (!line) // NULL line means Ctrl+D (EOF)
			break;
		if (*line) // Add non-empty lines to history
			add_history(line);
		ft_generate_commands(line, &comms); // Generate commands from input
		//args = parse_input(line);  // Tokenize the input string
		free(line); // Free the input line after parsing for it not to be stored on exit
		ft_run_commands(comms);
		// if (args && args[0]) // If we have a valid command
		// 	status = handle_command(shell, args); // Dispatch command
		// printf("executed\n");
		free_gc_cat(CAT_ARGS);
		free_gc_cat(CAT_TOKEN);
		free_gc_cat(CAT_CMD); // Free the arguments array
		// ft_free_split(args);
	}
	// Cleanup everything before exiting
	free_gc();
	//ft_free_split(shell->envp);
	clear_history();
	return status; // Return last command's status
}


