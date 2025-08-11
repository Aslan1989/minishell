/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:03:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/09 14:15:08 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Return 1 if env_var begins with key followed by '='.
 */
static int	match_key(const char *env_var, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (ft_strncmp(env_var, key, len) == 0 && env_var[len] == '=');
}

/**
 * @brief Decide whether an env string must be removed based on args list.
 */
int	should_remove(char *env_var, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i]) && match_key(env_var, args[i]))
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Count entries in a NULL-terminated env array.
 * @param envp Environment pointer array (may be NULL).
 * @return int Number of entries (0 if envp is NULL).
 */
int	env_count(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

/**
 * @brief Allocate a new env array with capacity for @p count entries + NULL.
 * Memory is tracked in CAT_ENV GC category.
 *
 * @param count Number of entries to hold (not including the final NULL).
 * @return char** Newly allocated array, or NULL on allocation failure.
 */
char	**env_alloc(int count)
{
	return (ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 1)));
}

/**
 * @brief Copy `src` into `dst` skipping keys listed in `args` (unset).
 *
 * For every removed entry we free its GC node via ft_gcfree(CAT_ENV, ...).
 * The destination is always NULL-terminated.
 *
 * @param src  Source env array.
 * @param args Unset arguments (args[1..]).
 * @param dst  Destination array (preallocated).
 */
void	env_copy_filtered(char **src, char **args, char **dst)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (!should_remove(src[i], args))
			dst[j++] = src[i];
		else
			ft_gcfree(CAT_ENV, src[i]);
		i++;
	}
	dst[j] = NULL;
}
