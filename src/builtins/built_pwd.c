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
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		ft_printf("%s\n", cwd);
	else
		perror("minishell: pwd");
	return (0);
}
