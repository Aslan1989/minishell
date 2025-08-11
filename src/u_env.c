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

void	free_env(char **envp)
{
	(void)envp;
	free_gc_cat(CAT_ENV);
}
