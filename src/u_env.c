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
 * @brief Copy an environment array into a new GC category.
 *
 * - Counts the number of entries in envp.
 * - Allocates space for (count + 1) pointers in the chosen category.
 * - Duplicates each string into the same category.
 *
 * Duplicate each "KEY=VALUE"
 *
 * @param envp Original environment array (NULL-terminated).
 * @param cat  Garbage collector category to allocate in.
 * @return char** Newly allocated copy, or NULL on error.
 */
char	**copy_env_cat(char **envp, t_egccat cat)
{
	int		count;
	int		i;
	char	**copy;

	count = 0;
	if (!envp)
		return (NULL);
	while (envp[count])
		count++;
	copy = ft_gcmalloc(cat, sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_gcstrdup(cat, envp[i]);
		if (!copy[i])
		{
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

/**
 * @brief Copy environment array into the CAT_ENV GC category.
 *
 * This is a specialized version of copy_env_cat() that always uses CAT_ENV.
 *
 * Duplicate each string
 * @param envp Original environment array (NULL-terminated).
 * @return char** New environment copy owned by CAT_ENV.
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
 * @brief Free all environment variables allocated in CAT_ENV.
 *
 * Since envp is GC-managed, we do not free each entry manually.
 * Instead, we free the entire GC category CAT_ENV.
 *
 * @param envp Environment array to free (ignored; category is global).
 */
void	free_env(char **envp)
{
	(void)envp;
	free_gc_cat(CAT_ENV);
}
