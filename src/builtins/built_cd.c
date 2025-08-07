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
 * @brief Implements the `cd` built-in command.
 *
 * Changes the current working directory to the path specified.
 *
 * @param args args[1] should contain the target directory.
 * @return int 0 on success, 1 on error (e.g., path does not exist).
 *
 * @example
 *   cd /home/user → changes to that directory
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

static char	*get_target_dir(t_shell *sh, char *arg)
{
	char	*oldpwd;

	if (ft_strcmp(arg, "-") == 0)
	{
		oldpwd = get_env_var(sh->envp, "OLDPWD");
		if (!oldpwd)
		{
			ft_print_error("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
		ft_printf("%s\n", oldpwd);
		return (oldpwd);
	}
	return (arg);
}

static int	update_pwd_vars(t_shell *sh, char *oldpwd)
{
	char	*cwd;

	cwd = get_current_dir();
	if (!cwd)
		return (1);
	if (!replace_env_var(sh->envp, "OLDPWD", oldpwd))
		add_env_var(sh, "OLDPWD", oldpwd);
	if (!replace_env_var(sh->envp, "PWD", cwd))
		add_env_var(sh, "PWD", cwd);
	return (0);
}

int	built_cd(char **args)
{
	t_shell	*sh;
	char	*oldpwd;
	char	*target;
	char	*cwd;

	sh = get_shell();
	if (!args[1])
		return (ft_printf("minishell: cd: missing argument\n"), 1);
	cwd = get_current_dir();
	if (!cwd)
		return (1);
	oldpwd = cwd;
	target = get_target_dir(sh, args[1]);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (update_pwd_vars(sh, oldpwd) != 0)
		return (1);
	return (0);
}
