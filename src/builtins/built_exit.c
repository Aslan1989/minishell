/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:36:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 15:55:57 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `exit` built-in command.
 *
 * Terminates the shell process. This is a direct call to `exit(0)`,
 * which stops the program immediately.
 *
 * @return int This function never returns, it exits the program.
 *
 * @example
 *   exit → closes the minishell
 */
int	built_exit(void)
{
	free_gc();
	clear_history();
	exit(0);
}
