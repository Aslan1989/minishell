/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:47:25 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 12:02:44 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Returns a pointer to the global shell structure.
 *
 * This function provides access to the shell's environment variables and
 * last exit status. It is used throughout the shell to manage state.
 *
 * @return t_shell* Pointer to the global shell structure.
 */
t_shell	*get_shell(void)
{
	static t_shell	sh;

	return (&sh);
}
