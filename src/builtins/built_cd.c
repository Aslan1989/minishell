/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:24:58 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 15:31:22 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Return a GC-managed duplicate of the current working directory.
 *
 * @return char* Newly allocated path on success, NULL on error.
 */
static char	*get_current_dir(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd (getcwd)");
		return (NULL);
	}
	return (ft_gcstrdup(CAT_ENV, cwd));
}

/**
 * @brief Resolve what directory `cd` should go to.
 *
 * Handles: no arg / "~" (HOME), "-" (OLDPWD), CDPATH, or raw arg.
 *
 * @param sh Shell state.
 * @param arg User argument (may be NULL).
 * @return char* Target directory (GC) or NULL on error.
 */
static char	*get_target_dir(t_shell *sh, const char *arg)
{
	char	*env_val;
	char	*from_cdpath;

	if (!arg || ft_strcmp(arg, "~") == 0)
	{
		env_val = get_env_var(sh->envp, "HOME");
		if (!env_val)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (ft_gcstrdup(CAT_ENV, env_val));
	}
	if (ft_strcmp(arg, "-") == 0)
		return (target_from_oldpwd(sh));
	from_cdpath = search_in_cdpath(sh, arg);
	if (from_cdpath)
		return (from_cdpath);
	return (ft_gcstrdup(CAT_ENV, arg));
}

/**
 * @brief Update PWD and OLDPWD after a successful chdir.
 *
 * @param sh Shell state.
 * @param oldpwd Previous PWD value.
 * @return int 0 on success, 1 on error.
 */
static int	update_pwd_vars(t_shell *sh, char *oldpwd)
{
	char	*cwd;

	if (!oldpwd)
		return (1);
	cwd = get_current_dir();
	if (!cwd)
		return (1);
	if (!replace_env_var(sh->envp, "OLDPWD", oldpwd))
		add_env_var(sh, "OLDPWD", oldpwd);
	if (!replace_env_var(sh->envp, "PWD", cwd))
		add_env_var(sh, "PWD", cwd);
	return (0);
}

static void	cd_print_error(char *arg)
{
	char	*msg;

	if (errno == ENOTDIR)
		msg = "not a directory";
	else if (errno == ENOENT)
		msg = "no such file or directory";
	else if (errno == EACCES)
		msg = "permission denied";
	else if (errno == ENAMETOOLONG)
		msg = "file name too long";
	else if (errno == ELOOP)
		msg = "too many levels of symbolic links";
	else
		msg = strerror(errno);
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(arg, STDERR_FILENO);
}

/**
 * @brief Builtin: cd
 *
 * Implements directory change with HOME, OLDPWD, and CDPATH support.
 * Applies POSIX-compatible diagnostic messages.
 *
 * @param args Command arguments (args[1] is the target or special token).
 * @return int 0 on success, 1 on failure.
 */
int	built_cd(char **args)
{
	t_shell	*sh;
	char	*oldpwd;
	char	*target;
	char	*cwd;

	sh = get_shell();
	cwd = get_current_dir();
	if (!cwd)
		return (1);
	oldpwd = ft_gcstrdup(CAT_ENV, cwd);
	if (!oldpwd)
		return (1);
	target = get_target_dir(sh, args[1]);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		cd_print_error(target);
		return (1);
	}
	if (update_pwd_vars(sh, oldpwd) != 0)
		return (1);
	return (0);
}
