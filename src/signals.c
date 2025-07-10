/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:53:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/09 13:35:03 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl-C).
 *
 * This function is called asynchronously when the user presses Ctrl-C.
 * It ensures the shell doesn't exit, but instead clears the current input line,
 * shows a new prompt on a new line, and allows the user to continue typing.
 *
 * - write(1, "\n", 1): prints a newline to move to the next line.
 * - rl_on_new_line(): tells readline that we're on a new line.
 * - rl_replace_line("", 0): clears the current line buffer.
 * - rl_redisplay(): redraws the prompt (e.g., "minishell$ ") cleanly.
 *
 * @param sig The signal number (usually SIGINT = 2).
 */
static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Set up signal handlers for interactive shell.
 *
 * This ensures:
 * - Ctrl-C (SIGINT) triggers our custom handler (handle_sigint).
 * - Ctrl-\ (SIGQUIT) is ignored completely.
 *
 * This makes the shell behave like bash:
 * - Ctrl-C: interrupts the current input and returns to prompt.
 * - Ctrl-\: does nothing.
 */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Disables echoing of control characters like ^C or ^\ in terminal.
 *
 * Useful for cleaner shell appearance when handling signals like SIGINT.
 */
void	disable_ctrl_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term); // Get current terminal attributes
	term.c_lflag &= ~ECHOCTL; // Disable echoing of control characters
	tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply changes immediately
}
