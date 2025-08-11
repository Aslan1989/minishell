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
 * @brief Get value of an environment name from envp (no allocation).
 * @param envp Environment.
 * @param name Variable name.
 * @return char* Pointer inside envp string after '=' or NULL.
 */
char	*get_env_var(char **envp, const char *name)
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
 * @brief Join directory and command into CAT_CMD GC.
 * @param dir PATH entry.
 * @param cmd Command name.
 * @return char* GC managed string (CAT_CMD) or NULL.
 */
static char	*ft_strjoin_path(const char *dir, const char *cmd)
{
	char	*full;
	char	*with_slash;

	if (!dir || !cmd)
		return (NULL);
	with_slash = ft_gcstrjoin(CAT_CMD, (char *)dir, "/");
	if (!with_slash)
		return (NULL);
	full = ft_gcstrjoin(CAT_CMD, with_slash, cmd);
	return (full);
}

/**
 * @brief Search cmd across PATH entries.
 * @param paths Split PATH array (malloc'ed by ft_split).
 * @param cmd Command name.
 * @return char* Full path in CAT_CMD on success, NULL otherwise.
 */
static char	*search_in_paths(char **paths, const char *cmd)
{
	char	*full_path;
	int		i;

	if (!paths || !cmd)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		ft_gcfree(CAT_CMD, full_path);
		i++;
	}
	return (NULL);
}

/**
 * @brief Resolve executable path using PATH if needed.
 * @param shell Shell (for envp).
 * @param cmd Command (may be name or contain '/').
 * @return char* CAT_CMD duplicate of resolved path or NULL if not found.
 */
char	*find_executable(t_shell *shell, const char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*dup;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_gcstrdup(CAT_CMD, cmd));
	path_env = get_env_var(shell->envp, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	dup = search_in_paths(paths, cmd);
	ft_free_split(paths);
	return (dup);
}
