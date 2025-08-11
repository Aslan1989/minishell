/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:31:51 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:01:55 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `pwd` built-in command.
 *
 * Prints the current working directory to the terminal.
 * Uses `getcwd()` to get the current path.
 *
 * @return int 0 on success, 1 on error.
 *
 * @example
 *   pwd → prints "/home/user/projects"
 */
int	built_pwd(void)
{
	t_shell	*sh;
	char	*pwd;
	char	cwd[PATH_MAX];

	sh = get_shell();
	pwd = get_env_var(sh->envp, "PWD");
	if (pwd && *pwd)
	{
		ft_putendl_fd(pwd, STDOUT_FILENO);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)))
		ft_putendl_fd(cwd, STDOUT_FILENO);
	else
		perror("minishell: pwd");
	return (0);
}
