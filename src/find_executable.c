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
 * @brief Join a directory and a command into a single path "dir/command".
 *
 * Uses GC-aware string join helper twice:
 *   1) join dir + "/"  → with_slash
 *   2) join with_slash + cmd → full
 *
 * @param dir Directory path (no trailing slash required).
 * @param cmd Command name (e.g., "ls").
 * @return char* Newly allocated "dir/cmd" or NULL on error/invalid input.
 */
static char	*ft_strjoin_path(const char *dir, const char *cmd)
{
	char	*full;
	char	*with_slash;

	if (!dir || !cmd)
		return (NULL);
	with_slash = ft_gcstrjoin(CAT_CMD, dir, "/");
	if (!with_slash)
		return (NULL);
	full = ft_gcstrjoin(CAT_CMD, with_slash, cmd);
	return (full);
}

/**
 * @brief Search an executable by trying each PATH directory.
 *
 * For each directory in `paths`, it builds "dir/cmd" and tests execute access.
 * Frees rejected candidates; returns the first match with X_OK.
 *
 * @param paths NULL-terminated array of directory strings (from PATH split).
 * @param cmd   Command name to locate (no slash inside).
 * @return char* Allocated absolute/relative path to an executable,
 * or NULL if not found or on error.
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
 * @brief Resolve an executable path for a command name.
 *
 * Resolution rules:
 *  - If cmd contains '/', treat it as a path and just duplicate it.
 *  - Else, read PATH from the environment, split by ':', and probe each dir.
 *
 * Memory:
 *  - Returned string is GC-allocated (CAT_CMD). Caller uses it directly.
 *
 * @param shell Shell state (to access envp / PATH).
 * @param cmd   Raw command name from argv[0].
 * @return char* Full path to executable or a duplicate of cmd if it has '/',
 *               NULL if PATH is missing, split fails, or nothing is found.
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
