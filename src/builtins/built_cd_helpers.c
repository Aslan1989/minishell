/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:31:39 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/19 12:31:35 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Replace KEY in the environment array with "KEY=VALUE", if present.
 * Iterates over the NULL-terminated @p envp array looking for an entry whose
 * name matches @p key (exact match before the '=' character). If found, frees
 * the old string via the GC (CAT_ENV) and stores a freshly built "KEY=VALUE"
 * string created by make_env_string().
 * @param envp  Mutable NULL-terminated array of "KEY=VALUE" strings managed
 *              under the CAT_ENV GC category. Must not be NULL.
 * @param key   Variable name to replace (must not contain '='). Must not be NULL
 *              or empty, otherwise matching becomes undefined.
 * @param value New value for the variable. Must not be NULL.
 * @return int
 * @retval 1 if an existing variable with name @p key was found and replaced.
 * @retval 0 if no such variable exists (nothing was changed).
 * @note This function does not add new variables. Use add_env_var() for that.
 * @warning On allocation failure inside make_env_string(), the current
 *          implementation will assign NULL to envp[i] and still return 1.
 *          Consider using a safer version that allocates first (see below).
 */
int	replace_env_var(char **envp, const char *key, const char *value)
{
	size_t	len;
	int		i;
	char	*new_str;

	if (!envp || !key || !*key || !value)
		return (0);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, len) && envp[i][len] == '=')
		{
			new_str = make_env_string(key, value);
			if (!new_str)
				return (-1);
			ft_gcfree(CAT_ENV, envp[i]);
			envp[i] = new_str;
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Check whether a path refers to an existing directory.
 * Calls stat(2) on @p path and returns 1 if the call succeeds and the
 * resulting file type is a directory. This function follows symbolic links,
 * i.e. a symlink pointing to a directory is treated as a directory.
 * @param path  Path to check (absolute or relative). Must not be NULL.
 * @return int
 * @retval 1 if @p path exists and is a directory (stat() == 0 && S_ISDIR()).
 * @retval 0 otherwise (stat() failed or the object is not a directory).
 * @note Errors from stat() (e.g., ENOENT, EACCES, ENOTDIR) are not
 *       distinguished; all map to 0.
 * @see stat(2), lstat(2), S_ISDIR
 */
static int	is_directory(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

/**
 * @brief Join a directory and a file name using '/' and register both buffers in GC.
 * Builds "dir/" and then "dir/file" using ft_strjoin(), registering both
 * intermediate and final buffers into the GC list under CAT_ENV. Returns the
 * final "dir/file" pointer.
 * @param dir   Directory path (no normalization is performed).
 * @param file  File or subdirectory name to append.
 * @return char*
 * @retval non-NULL Pointer to the GC-managed "dir/file" string on success.
 * @retval NULL     On allocation failure (the intermediate buffer, if any, is
 *                  still registered in the GC and will be freed later).
 */
static char	*join_path_gc(const char *dir, const char *file)
{
	const char	*base;
	char	*prefix;
	size_t	n;

	if (!file)
		return NULL;
	if (dir && *dir)
		base = dir;
	else
		base = ".";
	n = ft_strlen(base);
	if (n > 0 && base[n - 1] == '/')
		prefix = ft_gcstrdup(CAT_ENV, base);
	else
		prefix = ft_gcstrjoin(CAT_ENV, base, "/");
	if (!prefix)
		return (NULL);
	return (ft_gcstrjoin(CAT_ENV, prefix, file));
}

/**
 * @brief Try to resolve a `cd` target using CDPATH and print the resolved path.
 *
 * If $CDPATH is set, @p arg contains no '/' and does not start with '.',
 * iterate over the colon-separated directories in $CDPATH, join each with @p arg,
 * and return the first existing directory. On success, the resolved path is
 * printed to STDOUT (bash-like behavior).
 * @param sh   Shell handle (used to access the environment).
 * @param arg  The original `cd` argument (must not be NULL).
 * @return char*
 * @retval non-NULL GC-managed string with the resolved path if found.
 * @retval NULL     if CDPATH is not applicable or no directory matched.
 */
char	*search_in_cdpath(t_shell *sh, const char *arg)
{
	char	*cdpath;
	char	**paths;
	char	*full;
	int		i;

	cdpath = get_env_var(sh->envp, "CDPATH");
	if (!cdpath || !arg || !*arg || ft_strchr(arg, '/') || arg[0] == '.')
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
