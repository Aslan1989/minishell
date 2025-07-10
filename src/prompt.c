/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:53:10 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/09 13:35:23 by aisaev           ###   ########.fr       */
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
	return (readline("minishell$ "));
}
