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
int	built_cd(char **args)
{
	t_shell	*sh;
	char	cwd[PATH_MAX];
	char	*oldpwd;

	sh = get_shell();

	if (!args[1])
	{
		write(2, "minishell: cd: missing argument\n", 32);
		return (1);
	}
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd (getcwd)");
		return (1);
	}
	oldpwd = ft_gcstrdup(CAT_ENV, cwd);

	if (chdir(args[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}

	// update OLDPWD
	replace_env_var(sh->envp, "OLDPWD", oldpwd);

	// update PWD
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: cd (getcwd)");
		return (1);
	}
	replace_env_var(sh->envp, "PWD", cwd);

	return (0);
}

