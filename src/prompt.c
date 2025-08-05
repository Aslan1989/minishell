/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:53:10 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/11 13:01:49 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads a line from the user using readline.
 *
 * Displays a custom prompt (e.g., "minishell$ ") and waits for user input.
 * The readline function provides:
 * - Line editing (left/right arrows, backspace, etc.)
 * - Command history (arrow up/down)
 *
 * @return char* The string entered by the user. Must be freed manually.
 * Returns NULL if user sends EOF (Ctrl-D).
 */
char	*read_prompt(void)
{
	t_shell	*shell;

	shell = get_shell();
	if (shell->is_interactive)
		return (readline(COLOR_Y COLOR_BOLD"minishell$ "COLOR_X));
	return (get_next_line(STDIN_FILENO));
}
