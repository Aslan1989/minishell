/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:27:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/08 13:47:59 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Implements the `echo` built-in command.
 *
 * Prints strings to the terminal. Supports the `-n` option to suppress the newline.
 *
 * @param args Array of arguments. args[0] is "echo", args[1...] are strings.
 * @return int Always returns 0.
 *
 * @example
 *   echo Hello World → prints "Hello World"
 *   echo -n Hello → prints "Hello" without newline
 */
int built_echo(char **args)
{
	int i;
	int newline;

	i = 1;
	newline = 1; // By default, print newline at the end
	// Check for -n option(s), like -n or -nnnn
	while (args[i] && !ft_strncmp(args[i], "-n", 2) && \
			strspn(args[i] + 2, "n") == ft_strlen(args[i] + 2))
	{
		newline = 1; //print newline
		i++;
	}
	// Print all arguments separated by space
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n"); // Print newline if needed
	return (0);
}
