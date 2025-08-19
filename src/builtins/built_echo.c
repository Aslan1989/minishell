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
 * @brief Implement the `echo` builtin with POSIX-like -n handling.
 *
 * Parses one or more consecutive -n options (e.g., "-n", "-nnn"). Any token
 * that starts with '-' but contains characters other than 'n' after the dash
 * is treated as a normal argument (e.g., "-nfoo" is NOT an option).
 * Prints all remaining arguments separated by a single space.
 * If at least one -n was seen, the trailing newline is suppressed.
 *
 * @param args  argv-like array; args[0] is "echo", args[1..] are arguments.
 *              Must be NULL-terminated. args may be non-NULL with at least
 *              args[0] present in normal parser flow.
 * @return int
 * @retval 0 Always returns 0 (even on no-arg cases),
 * matching common shell behavior.
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
