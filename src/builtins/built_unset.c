/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:37:05 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:20:17 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_filter_env(char **envp, char ***new_env,
		const char *key, size_t key_len)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (envp[j])
	{
		if (!ft_strncmp(envp[j], key, key_len) && envp[j][key_len] == '=')
			ft_gcfree(CAT_ENV, envp[j]);
		else
			(*new_env)[k++] = envp[j];
		j++;
	}
	(*new_env)[k] = NULL;
}

/**
 * @brief Implements the `unset` built-in command.
 *
 * Removes variables from the shell's environment. For each argument passed,
 * it searches for the variable with the same name, and removes it from envp.
 *
 * @param shell Pointer to the shell structure.
 * @param args Array of arguments, where args[0] is "unset", and args[1...]
 * are keys.
 * @return int Always returns 0 on success.
 *
 * @example
 *   unset PATH → removes the PATH variable from envp.
 */
int	built_unset(t_shell *shell, char **args)
{
	int			i;
	int			count;
	const char	*key;
	size_t		key_len;
	char		**new_env;

	i = 1;
	while (args[i])
	{
		key = args[i];
		key_len = ft_strlen(key);
		count = 0;
		while (shell->envp[count])
			count++;
		new_env = ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 1));
		if (!new_env)
			return (1);
		ft_filter_env(shell->envp, &new_env, key, key_len);
		ft_gcfree(CAT_ENV, shell->envp);
		shell->envp = new_env;
		i++;
	}
	return (0);
}
