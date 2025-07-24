/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:37:05 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/24 11:10:26 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `unset` built-in command.
 *
 * Removes variables from the shell's environment. For each argument passed,
 * it searches for the variable with the same name, and removes it from envp.
 *
 * @param shell Pointer to the shell structure.
 * @param args Array of arguments, where args[0] is "unset", and args[1...] are keys.
 * @return int Always returns 0 on success.
 *
 * @example
 *   unset PATH → removes the PATH variable from envp.
 */
int built_unset(t_shell *shell, char **args)
{
	int i;
	int count;
	int j;
	int k;
	const char *key;
	size_t key_len;
	char **new_env;

	i = 1; // Start from args[1], because args[0] is "unset"
	while (args[i])
	{
		key = args[i];
		key_len = ft_strlen(key);
		count = 0;
		// Count how many env variables exist
		while (shell->envp[count])
			count++;
		new_env = ft_gcmalloc(CAT_ENV, sizeof(char *) * count); // New env without 1 var
		if (!new_env)
			return 1;
		j = 0; // index for old envp
		k = 0; // index for new_env
		while (shell->envp[j])
		{
			// If variable matches the key and has '=' immediately after
			if (!ft_strncmp(shell->envp[j], key, key_len) && \
			shell->envp[j][key_len] == '=')
				ft_gcfree(CAT_ENV, shell->envp[j]); // Match found – free and skip this entry
			else
				new_env[k++] = shell->envp[j]; // Keep this variable
			j++;
		}
		new_env[k] = NULL;
		// Replace the old environment with the new one
		ft_gcfree(CAT_ENV, shell->envp);
		shell->envp = new_env;
		i++; // Move to next key
	}
	return (0);
}
