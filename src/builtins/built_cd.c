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
 * @brief Compute the target directory for the `cd` command.
 * Resolution order:
 *  - If @p arg is NULL or equals "~", resolve to $HOME (error if unset).
 *  - If @p arg equals "-", resolve via OLDPWD (may print an error inside).
 *  - Otherwise, try CDPATH search (applies only when arg has no '/' and
 *    does not start with '.'); if found, return that directory.
 *  - Fallback: return a GC-managed duplicate of @p arg.
 * @param sh   Shell handle (provides access to the environment).
 * @param arg  User-provided `cd` argument (may be NULL).
 * @return char*
 * @retval non-NULL GC-managed path to attempt with chdir().
 * @retval NULL     if resolution failed (e.g., HOME not set, OLDPWD not set).
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
 * @brief Update the shell's OLDPWD and PWD after a successful directory change.
 * Expects @p oldpwd to contain the previous working directory (captured
 * before chdir()). The function retrieves the current working directory
 * via get_current_dir() and then:
 *  - replaces OLDPWD with @p oldpwd or adds it if missing;
 *  - replaces PWD with the new cwd or adds it if missing.
 * @param sh     Shell handle with environment.
 * @param oldpwd Previous working directory (GC-managed string), must not be NULL
 * @return int
 * @retval 0 on success.
 * @retval 1 on failure (e.g., @p oldpwd is NULL or get_current_dir() failed).
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
 * @brief Implement the `cd` builtin.
 *
 * Semantics:
 *  - With no args or "~": change to $HOME (error if unset).
 *  - With "-": change to $OLDPWD (error if unset).
 *  - Otherwise: if CDPATH applies, resolve via CDPATH, else use the argument.
 *  - On success, update OLDPWD and PWD to the physical paths.
 *  - Print the resolved path to STDOUT only when:
 *      * the argument was "-", or
 *      * the directory was resolved via CDPATH,
 *    and only after a successful chdir().
 *  - With more than one non-option argument, print "too many arguments" and
 *    return 1 without changing directory.
 *
 * @param args argv-style array for the command; args[0] == "cd".
 * @return 0 on success; 1 on error.
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
