/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:46:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/09 13:16:44 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Splits the input string into individual arguments.
 *
 * Handles spaces and quotes correctly. Calls helper functions to:
 *  - count how many arguments there are,
 *  - extract each one from the string.
 *
 * @param line Input string typed by the user (e.g., "echo 'hi 42'")
 * @return char** NULL-terminated array of argument strings.
 */
char	**parse_input(const char *line)
{
	int		argc;
	int		i;
	char	**args;

	if (!line || !*line)
		return (NULL);
	argc = count_args(line);
	args = malloc(sizeof(char *) * (argc + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*line && i < argc)
		args[i++] = extract_arg(&line);
	args[i] = NULL;
	return (args);
}
