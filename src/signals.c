/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:53:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:30:58 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Custom handler for SIGINT (Ctrl-C).
 *
 * Behavior in interactive mode:
 *  - Print a newline (so prompt starts on a new line).
 *  - Reset the readline buffer to empty.
 *  - Redisplay a clean prompt.
 * Avoid unused parameter warning
 * Print newline after Ctrl-C
 * Tell readline we are on a new line
 * Clear the current input buffer
 * Show prompt again
 *
 * @param sig Signal number (unused here).
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	//get_shell()->last_exit_status = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Setup signal handlers for interactive shell.
 *
 * - SIGINT (Ctrl-C) → calls our custom handler (clears line, new prompt).
 * - SIGQUIT (Ctrl-\) → ignored (no "Quit" message, like in bash).
 * Install handler for Ctrl-C
 * Ignore Ctrl-\ completely
 */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	parent_signals_exec_begin(void)
{
	if (get_shell()->is_interactive)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}

void	parent_signals_exec_end(void)
{
	if (get_shell()->is_interactive)
		setup_signals();
}

/**
 * @brief Disable printing of control characters (like "^C") on terminal.
 *
 * Modifies terminal attributes so that when Ctrl-C is pressed,
 * the `^C` is not echoed. This makes behavior match bash/zsh.
 * Get current terminal attributes
 * Disable echo of control characters
 * Apply changes immediately
 */
void	disable_ctrl_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
