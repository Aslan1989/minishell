/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:26:19 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/10 15:00:40 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Searches for an environment variable by name and returns its value.
 *
 * For example, if the envp array contains: "PATH=/usr/bin:/bin",
 * and name = "PATH", this function returns the string "/usr/bin:/bin".
 *
 * @param envp Array of strings, each in the form "KEY=VALUE".
 * @param name The name of the variable to find (e.g., "PATH").
 * @return char* Pointer to the value part of the matching variable,
 * or NULL if the variable is not found.
 *
 * @example
 * get_env_var(envp, "PATH") → "/usr/bin:/bin"
 */
static char	*get_env_var(char **envp, const char *name)
{
	size_t len;
	int i;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return NULL;
}

/**
 * @brief Searches for a command executable file in the system's PATH.
 *
 * When a user types a command like `ls` or `cat`, this function looks through all
 * the directories listed in the PATH environment variable to find where the command
 * file is located (e.g., `/bin/ls`). If it finds a valid file that is executable,
 * it returns its full path so it can be executed using `execve`.
 *
 * @param shell Pointer to the shell structure containing environment variables.
 * @param cmd The command name to search for (e.g., "ls", "grep").
 * @return char* The full path to the executable file, or NULL if not found.
 *
 * @example
 *   find_executable(shell, "ls") → "/bin/ls"
 *   find_executable(shell, "badcmd") → NULL
 */
char *find_executable(t_shell *shell, const char *cmd)
{
	char *path_env; // Will store the content of PATH variable
	char **paths; // Will store individual folders from PATH (split by ':')
	char *full_path;
	char *candidate;
	int i;
	int j;
	// Check if command is NULL or empty (e.g., user presses Enter with no input)
	if (!cmd || !*cmd)
		return NULL;
	// Get the value of the PATH environment variable
	path_env = get_env_var(shell->envp, "PATH");
	// If PATH is not defined, we can't search for the command
	if (!path_env)
		return NULL;
	// Split the PATH string by ':' to get all directory paths
	paths = ft_split(path_env, ':'); // Example: "/usr/bin:/bin:/usr/local/bin"
	if (!paths)
		return NULL;
	i = 0;
	while (paths[i])
	{
		// Build the full path like: /bin/ + ls = /bin/ls
		full_path = ft_strjoin(paths[i], "/");
		candidate = ft_strjoin(full_path, cmd);
		free(full_path); // free intermediate string
		// Check if the file exists and is executable
		if (candidate && access(candidate, X_OK) == 0)
		{
			// Free all other memory and return the working path
			j = 0;
			while (paths[j])
			{
				free(paths[j]);
				j++;
			}
			free(paths);
			return candidate; // return the found path like "/bin/ls"
		}
		// If the file was not found or is not executable, try the next one
		free(candidate);
		i++;
	}
	// If none of the paths worked, clean up and return NULL
	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	return NULL;
}
