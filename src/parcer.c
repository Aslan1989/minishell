/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:46:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/22 18:05:22 by psmolin          ###   ########.fr       */
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
// char	**parse_input(const char *line)
// {
// 	int		argc;
// 	int		i;
// 	char	**args;

// 	if (!line || !*line)
// 		return (NULL);
// 	argc = count_args(line);
// 	// args = malloc(sizeof(char *) * (argc + 1));
// 	args = ft_gcmalloc(CAT_ARGS, sizeof(char *) * (argc + 1));
// 	if (!args)
// 		return (NULL);
// 	i = 0;
// 	while (*line && i < argc)
// 	{
// 		args[i++] = extract_arg(&line);
// 	}
// 	args[i] = NULL;
// 	return (args);
// }

int	parse_input(t_cmd *node, const char *line)
{
	int		argc;
	int		i;
	char	*arg;

	if (!line || !*line)
		return (node->commands = NULL, 1);
	argc = count_args(line);
	node->commands = ft_gcmalloc(CAT_ARGS, sizeof(char *) * (argc + 1));
	if (!node->commands)
		return (1);
	i = 0;
	// printf(COLOR_B"arg count: %d\n"COLOR_X, argc);
	while (*line && i < argc)
	{
		arg = extract_arg(&line);
		if (!arg)
			break;
		if (ft_strcmp(arg, "<") == 0)
		{
			node->infile = 1;
			// printf(COLOR_R"infile: %s\n"COLOR_X, arg);
			node->infile_name = extract_arg(&line);
		} else if (ft_strcmp(arg, ">") == 0)
		{
			node->outfile = 1;
			// printf(COLOR_R"outfile: %s\n"COLOR_X, arg);
			node->outfile_name = extract_arg(&line);
		}
		else if (ft_strcmp(arg, ">>") == 0)
		{
			node->outfile = 1;
			node->append = 1;
			node->outfile_name = extract_arg(&line);
		}
		else if (ft_strcmp(arg, "<<") == 0)
			node->heredoc = 1;
		else
			node->commands[i++] = arg; // Store the argument in the commands array
		// printf(COLOR_G"arg[%d]: %s\n"COLOR_X, i, arg);
	}
	if ((node->infile && !node->infile_name)
		|| (node->outfile && !node->outfile_name))
	{
		ft_print_error("minishell: syntax error near unexpected token `newline'\n");
		node->commands = NULL;
		return (1);
	}
	// while (i < argc)
	// 	node->commands[i++] = NULL;
	node->commands[i] = NULL; // NULL-terminate the commands array
	while (i <= argc)
	{
		node->commands[i++] = NULL; // Fill remaining slots with NULL
	}
	// i = 0;
	// while (i <= argc)
	// {
	// 	printf(COLOR_Y"commands[%d]: %s\n"COLOR_X, i, node->commands[i]);
	// 	i++;
	// }
	return (0);
}
