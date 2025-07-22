/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:36:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/22 16:16:11 by psmolin          ###   ########.fr       */
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
int built_exit(void)
{
	//ft_free_split(args);
	// free_gc_cat(CAT_ARGS);
	// free_gc_cat(CAT_ENV);
	free_gc(); // Free all allocated memory in garbage collector
	//ft_free_split(get_shell()->envp);
	clear_history();
	exit(0);
}
