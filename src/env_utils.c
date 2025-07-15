/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:20:16 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/15 23:24:26 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a deep copy of the environment variables.
 *
 * When the shell starts, it receives a list of environment variables from the OS.
 * We create a separate copy so we can modify it (for export/unset) without affecting the system.
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
	// First, count how many environment variables there are
	while (envp[count])
		count++;
// Allocate memory for the new environment array (plus one for the NULL at the end)

	//////////////////!
	//copy = malloc(sizeof(char *) * (count + 1));
	copy = ft_gcmalloc(CAT_ENV, sizeof(char *) * (count + 1));
	if (!copy)
		return NULL;
	// Duplicate each string using strdup (which copies string into new memory)
	i = 0;
	while (i < count)
	{
		// copy[i] = ft_strdup(envp[i]); // "PATH=/usr/bin" → copy[i] = "PATH=/usr/bin"
		copy[i] = ft_gcstrdup(CAT_ENV, envp[i]);
		i++;
	}
	copy[count] = NULL; // Important: NULL terminator for arrays of strings
	return copy;
}

/**
 * @brief Frees memory allocated for environment variables.
 *
 * This function is used when exiting the shell to clean up memory.
 * It goes through each string in the array and frees it, then frees the array itself.
 *
 * @param envp The environment variable array to free.
 */
void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return;
	while (envp[i])
	{
		free(envp[i]); // Free each string
		i++;
	}
	free(envp); // Finally, free the array of pointers
}
