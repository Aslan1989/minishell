/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:24:58 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/08 13:48:19 by aisaev           ###   ########.fr       */
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
int built_cd(char **args)
{
	if (!args[1])
	{
		// No argument provided: cd requires a path
		write(2, "minishell: cd: missing argument\n", 32);
		return (1);
	}
	if (chdir(args[1]) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	return (0);
}
