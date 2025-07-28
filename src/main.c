/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:20:42 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:30:13 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_initialize_shell(char **envp)
{
	t_shell	*shell;

	shell = get_shell();
	shell->envp = copy_env(envp);
	shell->last_exit_status = 0;
	setup_signals();
	ft_print_banner();
}

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
int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		status;
	t_cmd	*comms;

	ft_ignore_ac_av(argc, argv);
	comms = NULL;
	status = 0;
	ft_initialize_shell(envp);
	while (1)
	{
		line = read_prompt();
		if (!line)
			break ;
		if (*line)
			add_history(line);
		ft_generate_commands(line, &comms);
		free(line);
		ft_run_commands(comms);
		free_gc_cat(CAT_ARGS);
		free_gc_cat(CAT_TOKEN);
		free_gc_cat(CAT_CMD);
	}
	free_gc();
	clear_history();
	return (status);
}
