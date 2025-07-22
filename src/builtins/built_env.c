/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:35:02 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/22 18:09:53 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `env` built-in command.
 *
 * Prints all environment variables currently stored in the shell.
 *
 * @param shell Pointer to the shell structure holding envp.
 * @return int 0 on success, 1 on error.
 *
 * @example
 *   env → prints list like "PATH=/usr/bin", etc.
 */
int built_env(t_shell *shell)
{
	int i;

	i = 0;
	if (!shell || !shell->envp)
		return 1;
	while(shell->envp[i])
	{
		ft_printf("%s\n", shell->envp[i]);
		i++;
	}
	return 0;
}
