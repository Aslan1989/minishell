/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:20:16 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/24 12:49:29 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a deep copy of the environment variables.
 *
 * When the shell starts, it receives a list of environment variables
 * from the OS. We create a separate copy so we can modify it (for export/unset)
 * without affecting the system.
 *
 * @param envp The original environment variable array passed to main().
 * @return char** A NULL-terminated copy of the environment variable strings.
 */
char	**copy_env(char **envp)
{
	int		count;
	char	**copy;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	copy = ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_gcstrdup(CAT_ENV, envp[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

/**
 * @brief Frees memory allocated for environment variables.
 */
void	free_env(char **envp)
{
	(void)envp;
	free_gc_cat(CAT_ENV);
}
