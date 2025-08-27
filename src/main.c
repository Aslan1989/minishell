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

/**
 * @brief Initialize global shell state from inherited environment.
 *
 * Copies envp, resets status flags, detects interactive mode (isatty),
 * and, if interactive, sets up signals, disables ^C echo, and prints a banner.
 * Get the global shell singleton
 * Deep-copy environment (owned by our shell)
 * True if input comes from a terminal
 * Mark envp as dynamically owned by us
 * No pending parser error at startup
 * Extra setup only in interactive mode
 * Install custom handlers for SIGINT/SIGQUIT
 * Turn off terminal echo for control chars
 * Optional greeting/banner
 *
 * @param envp Null-terminated environment array inherited
 * from the parent process.
 */
static void	ft_initialize_shell(char **envp)
{
	t_shell	*shell;

	shell = get_shell();
	shell->envp = copy_env(envp);
	shell->last_exit_status = 0;
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->envp_allocated = 1;
	shell->syntax_error = 0;
	shell->heredoc_interrupted = 0;
	if (shell->is_interactive)
	{
		ft_print_banner();
		setup_signals();
		disable_ctrl_echo();
	}
}

/**
 * @brief Interactive REPL loop: read, parse, execute, repeat.
 *
 * Reads lines until EOF (Ctrl-D). Adds non-empty lines to history.
 * After each iteration, frees per-command GC categories to avoid leaks.
 *
 * @return int Exit status of the last executed command before exit.
 * Main REPL loop
 * Show prompt and read a line (may be NULL on EOF)
 * User pressed Ctrl-D or input closed
 * Only store non-empty lines
 * Append to readline history
 * Build AST for this input line
 * Free the raw line (AST keeps needed data)
 * Execute and get exit status
 * Update $? for the next prompt
 * Arguments/expanded tokens
 * Token list
 * AST nodes
 *
 * @return int Exit status of the last executed command before exit.
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

/**
 * @brief Program entry point: decides between interactive
 * and non-interactive modes.
 *
 * Usage:
 *   minishell -c "echo hi"   → non-interactive one-shot
 *   minishell                 → interactive loop
 * POSIX-style "-c <cmd>" one-shot
 * Prepare interactive shell
 * Enter REPL
 * Final GC cleanup on exit
 * Free readline history
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param envp Inherited environment.
 * @return int Process exit status.
 */
int	main(int argc, char **argv, char **envp)
{
	int		status;

	(void)argv;
	if (argc >= 2)
	{
		ft_putendl_fd("minishell: too many arguments", STDERR_FILENO);
		return (2);
	}
	ft_initialize_shell(envp);
	status = run_interactive_loop();
	free_gc();
	clear_history();
	return (status);
}
