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
 * @brief Get the current working directory as a GC-managed string.
 * Calls getcwd(3) into a PATH_MAX-sized stack buffer and returns a GC-managed
 * duplicate (CAT_ENV) on success. On failure, prints a diagnostic via perror()
 * with the "minishell: cd (getcwd)" prefix and returns NULL.
 * @return char*
 * @retval non-NULL A GC-managed duplicate of the current directory path.
 * @retval NULL     If getcwd() fails; an error message is already printed.
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
 * @brief Resolve the target directory for `cd`.
 *
 * Resolution order:
 *  - If arg is NULL or "~": use $HOME (error if not set).
 *  - If arg is "-": use $OLDPWD (and usually print it in the caller, if needed).
 *  - If arg is found via $CDPATH: use that path.
 *  - Otherwise: use arg as-is.
 *
 * @param sh  Shell state (access to envp).
 * @param arg Raw user argument (may be NULL).
 * @return char* Newly allocated target path, or NULL on error.
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
 * @brief Update PWD and OLDPWD environment variables after a successful chdir().
 *
 * Logic:
 *  - Get the new CWD (absolute).
 *  - Replace or add OLDPWD with the provided oldpwd.
 *  - Replace or add PWD with the new CWD.
 *
 * @param sh     Shell state with envp and env helpers.
 * @param oldpwd The previous working directory (must be a valid string).
 * @return int 0 on success, 1 on any failure.
 */
static int	update_pwd_vars(t_shell *sh, char *oldpwd)
{
	char	*cwd;
	int		r;

	if (!oldpwd)
		return (1);
	cwd = get_current_dir();
	if (!cwd)
		return (1);
	r = replace_env_var(sh->envp, "OLDPWD", oldpwd);
	if (r == -1)
		return (1);
	if (r == 0 && add_env_var(sh, "OLDPWD", oldpwd) != 0)
		return (1);
	r = replace_env_var(sh->envp, "PWD", cwd);
	if (r == -1)
		return (1);
	if (r == 0 && add_env_var(sh, "PWD", cwd) != 0)
		return (1);
	return (0);
}

/**
 * @brief Print a bash-compatible cd error: "minishell: cd: <arg>: <Message>".
 *
 * Captures errno, then prints "minishell: cd: <arg>: <strerror(errno)>"
 * to STDERR.
 * Do not call any function that could overwrite errno before capturing it.
 *
 * @param arg  The path argument that failed (can be NULL; then only
 * the message is printed).
 */
static void	cd_print_error(char *arg)
{
	int	err;

	err = errno;
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	if (arg && *arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(strerror(err), STDERR_FILENO);
}

/**
 * @brief Builtin `cd`: change the current working directory.
 *
 * Steps:
 *  1) Remember current directory (for OLDPWD).
 *  2) Resolve the target directory from args and env
 * 	   (HOME, OLDPWD, CDPATH, "~", "-").
 *  3) chdir() to the target.
 *  4) Update PWD and OLDPWD in the environment.
 *
 * @param args Command arguments. args[1] is the desired target path.
 * @return int 0 on success, 1 on error.
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
	oldpwd = cwd;
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
