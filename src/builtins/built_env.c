/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:35:02 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 15:55:41 by psmolin          ###   ########.fr       */
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
int	built_env(t_shell *shell, char **args)
{
	int	i;

	if (!shell || !shell->envp)
		return (1);
	if (args[1])
	{
		ft_putstr_fd("env: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (127);
	}
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
			ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}
