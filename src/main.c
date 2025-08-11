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
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->envp_allocated = 1;
	if (shell->is_interactive)
	{
		setup_signals();
		disable_ctrl_echo();
		ft_print_banner();
	}
}

/* already in your file — оставь как есть
static void ft_initialize_shell(char **envp) { ... }
*/

/**
 * @brief Run one-shot non-interactive command: minishell -c "cmd".
 * Initializes shell, runs, cleans up, returns exit status.
 */
static int	run_noninteractive(char **envp, const char *cmd)
{
	t_shell	*sh;
	char	*line;
	t_cmd	*comms;
	int		status;

	sh = get_shell();
	sh->is_interactive = 0;
	ft_initialize_shell(envp);
	line = ft_gcstrdup(CAT_MEM, cmd);
	if (!line)
		return (1);
	comms = NULL;
	ft_generate_commands(line, &comms);
	status = ft_run_commands(comms);
	sh->last_exit_status = status;
	free_gc();
	clear_history();
	return (status);
}

/**
 * @brief Interactive REPL loop: read → parse → exec → GC category cleanup.
 * Breaks on EOF (Ctrl-D). Returns last command status.
 */
static int	run_interactive_loop(void)
{
	t_shell	*sh;
	char	*line;
	t_cmd	*comms;
	int		status;

	sh = get_shell();
	status = 0;
	comms = NULL;
	while (1)
	{
		line = read_full_prompt();
		if (!line)
			break ;
		if (*line && sh->is_interactive)
			add_history(line);
		ft_generate_commands(line, &comms);
		free(line);
		status = ft_run_commands(comms);
		sh->last_exit_status = status;
		free_gc_cat(CAT_ARGS);
		free_gc_cat(CAT_TOKEN);
		free_gc_cat(CAT_CMD);
	}
	return (status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*sh;
	int		status;

	sh = get_shell();
	if (argc >= 3 && strcmp(argv[1], "-c") == 0)
		return (run_noninteractive(envp, argv[2]));
	ft_initialize_shell(envp);
	status = run_interactive_loop();
	free_gc();
	clear_history();
	return (status);
}
