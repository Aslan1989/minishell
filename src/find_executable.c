/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:26:19 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 15:31:12 by psmolin          ###   ########.fr       */
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
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * @brief Searches for a command executable file in the system's PATH.
 *
 * When a user types a command like `ls` or `cat`, this function looks
 * through all the directories listed in the PATH environment variable to
 * find where the command file is located (e.g., `/bin/ls`).
 * If it finds a valid file that is executable,
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
char	*find_executable(t_shell *shell, const char *cmd)
{
	char	**paths;
	char	*full_path;
	char	*candidate;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	paths = ft_split(get_env_var(shell->envp, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], "/");
		candidate = ft_strjoin(full_path, cmd);
		free(full_path);
		if (candidate && access(candidate, X_OK) == 0)
			return (ft_free_split(paths), candidate);
		free(candidate);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}
