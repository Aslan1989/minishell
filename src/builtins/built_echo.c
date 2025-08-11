/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:27:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/31 13:47:41 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `echo` built-in command.
 *
 * Prints strings to the terminal. Supports the `-n` option to suppress the
 * newline.
 *
 * @param args Array of arguments. args[0] is "echo", args[1...] are strings.
 * @return int Always returns 0.
 *
 * @example
 *   echo Hello World → prints "Hello World"
 *   echo -n Hello → prints "Hello" without newline
 */
int	built_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n'
		&& ft_strspn(args[i] + 1, "n") == ft_strlen(args[i] + 1))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
