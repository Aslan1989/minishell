/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   units.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:00:44 by marvin            #+#    #+#             */
/*   Updated: 2025/08/09 14:36:04 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Build "KEY=VALUE" as a GC-managed string.
 *
 * @param key Key part.
 * @param value Value part.
 * @return char* Newly allocated string or NULL.
 */
char	*make_env_string(const char *key, const char *value)
{
	size_t	key_len;
	size_t	val_len;
	char	*env_str;
	size_t	i;
	size_t	j;

	key_len = ft_strlen(key);
	val_len = ft_strlen(value);
	env_str = ft_gcmalloc(CAT_ENV, key_len + val_len + 2);
	i = 0;
	j = 0;
	if (!env_str)
		return (NULL);
	while (i < key_len)
	{
		env_str[i] = key[i];
		i++;
	}
	env_str[i++] = '=';
	while (j < val_len)
		env_str[i++] = value[j++];
	env_str[i] = '\0';
	return (env_str);
}

/**
 * @brief Append KEY=VALUE at the end of shell->envp.
 *
 * @param shell Shell state (owns envp GC category).
 * @param key Key to add.
 * @param value Value to add.
 * @return int 0 on success, 1 on allocation failure.
 */
int	add_env_var(t_shell *shell, const char *key, const char *value)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (shell->envp[count])
		count++;
	new_env = ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->envp[i];
		i++;
	}
	new_env[count] = make_env_string(key, value);
	new_env[count + 1] = NULL;
	ft_gcfree(CAT_ENV, shell->envp);
	shell->envp = new_env;
	return (0);
}
