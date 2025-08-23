/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:03:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 12:10:48 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Return 1 if env_var begins with key followed by '='.
 * Compute the length of key.
 * Compare the first len chars of env_var to key.
 * Ensure the next character in env_var is '='
 */
static int	match_key(const char *env_var, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (ft_strncmp(env_var, key, len) == 0 && env_var[len] == '=');
}

/**
 * @brief Decide whether an env string must be removed based on args list.
 * For each argument: if it is a valid identifier
 * and the key matches this env_var, we should remove it.
 * If no matches, keep it.
 */
static int	should_remove(char *env_var, char **args)
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
 * Iterate all source entries.
 * If a variable must stay, copy its pointer to dst and advance j.
 * If it must be removed, free that string via GC
 * (the entry itself, not the whole array).
 * Null-terminate the destination.
 * Result: dst reuses surviving entries; removed ones are freed.
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
