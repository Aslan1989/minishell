/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:31:39 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/17 18:33:21 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Replace KEY in envp with KEY=VALUE, if present.
 *
 * @param envp Environment array (GC-managed entries).
 * @param key Key to replace.
 * @param value New value.
 * @return int 1 if replaced, 0 if not found.
 */
int	replace_env_var(char **envp, const char *key, const char *value)
{
	size_t	len;
	int		i;

	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			ft_gcfree(CAT_ENV, envp[i]);
			envp[i] = make_env_string(key, value);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Check whether the given path is an existing directory.
 *
 * @param path Path to check.
 * @return int 1 if directory exists, otherwise 0.
 */
static int	is_directory(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

/**
 * @brief Join two path parts using '/'. Both results are GC-managed.
 *
 * @param dir Directory part.
 * @param file Trailing component.
 * @return char* Joined string or NULL on error.
 */
static char	*join_path_gc(const char *dir, const char *file)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	ft_gc_addback(get_gc(CAT_ENV), tmp);
	full = ft_strjoin(tmp, file);
	if (!full)
		return (NULL);
	ft_gc_addback(get_gc(CAT_ENV), full);
	return (full);
}

/**
 * @brief Try to resolve argument using CDPATH (POSIX behaviour).
 *
 * Prints the final path when CDPATH is used (like bash).
 *
 * @param sh Shell state (for envp).
 * @param arg Raw cd argument (no '/', not starting with '.').
 * @return char* Resolved path (GC) or NULL if not found/applicable.
 */
char	*search_in_cdpath(t_shell *sh, const char *arg)
{
	char	*cdpath;
	char	**paths;
	char	*full;
	int		i;

	cdpath = get_env_var(sh->envp, "CDPATH");
	if (!cdpath || ft_strchr(arg, '/') || ft_strncmp(arg, ".", 1) == 0)
		return (NULL);
	paths = ft_split(cdpath, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full = join_path_gc(paths[i], arg);
		if (full && is_directory(full))
		{
			ft_putendl_fd(full, STDOUT_FILENO);
			ft_free_split(paths);
			return (full);
		}
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

/**
 * @brief Resolve OLDPWD (-) target, echo it (bash behavior), or error.
 * @return char* GC-managed path or NULL if OLDPWD not set.
 */
char	*target_from_oldpwd(t_shell *sh)
{
	char	*oldpwd;

	oldpwd = get_env_var(sh->envp, "OLDPWD");
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(oldpwd, STDOUT_FILENO);
	return (ft_gcstrdup(CAT_ENV, oldpwd));
}
