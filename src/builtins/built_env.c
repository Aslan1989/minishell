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
 * @brief Implement the `env` builtin (print environment).
 *
 * Prints all entries from the shell environment that contain '='
 * in the form "KEY=VALUE", one per line, to STDOUT. If any extra
 * argument is provided, this simplified minishell implementation
 * does not support `env KEY=VAL ...` or `env cmd` forms: it prints
 * an error and returns 127.
 * @param shell  Shell handle containing the environment (shell->envp).
 * @param args   argv-like array; args[0] is "env", args[1] (if present)
 *               triggers the error path in this implementation.
 * @return int
 * @retval 0   On success (environment printed, possibly empty).
 * @retval 1   If shell or shell->envp is NULL.
 * @retval 127 If extra arguments are provided (unsupported usage).
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
