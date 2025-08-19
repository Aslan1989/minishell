/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:46:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/04 23:23:45 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_result(char ***res, int *count, const char *s)
{
	char	**r;

	r = (char **)ft_gcrealloc(CAT_ARGS, *res, sizeof(char *) * (*count + 2));
	if (!r)
		return (1);
	r[*count] = ft_gcstrdup(CAT_ARGS, s);
	if (!r[*count])
		return (1);
	(*count)++;
	r[*count] = NULL;
	*res = r;
	return (0);
}

/**
 * @brief Takes an argument list and expands any wildcards.
 *
 * This function uses globbing to expand patterns like `*`, `?`, and `[]`.
 * It replaces the original arguments with the expanded list.
 * @param args The array of arguments to expand.
 * @return char** A new array of arguments with wildcards expanded.
 */
char	**ft_expand_wildcards(t_arg **args)
{
	char	**result;
	int		count;
	int		i;
	size_t	j;
	glob_t	g;

	result = NULL;
	count = 0;
	i = 0;
	while (args && args[i])
	{
		if (args[i]->wildcard && ft_strpbrk(args[i]->arg, "*?[]")
			&& glob(args[i]->arg, 0, NULL, &g) == 0)
		{
			j = 0;
			while (j < g.gl_pathc)
				if (append_result(&result, &count, g.gl_pathv[j++]))
					return (globfree(&g), result);
			globfree(&g);
		}
		else if (append_result(&result, &count, args[i]->arg))
			return (result);
		i++;
	}
	return (result);
}

/**
 * @brief Fill command->args with parsed t_arg* and redirections.
 *
 * @param node Command node to fill.
 * @param line Original token text.
 * @param argc Pre-counted number of arguments.
 * @param i Out: number of non-redirection args collected.
 * @return int 0 on success, 1 on error.
 */
static int	ft_fillout_commands(t_cmd *node, const char *line, int argc, int *i)
{
	t_arg	*arg;
	t_arg	*next;
	int		check;

	*i = 0;
	check = 0;
	while (*line && *i < argc)
	{
		arg = extract_arg(&line);
		if (!arg)
			return (1);
		if (!ft_strcmp(arg->arg, "<") || !ft_strcmp(arg->arg, ">")
			|| !ft_strcmp(arg->arg, ">>") || !ft_strcmp(arg->arg, "<<"))
		{
			next = extract_arg(&line);
			if (!next || ft_redir_check_next(next->arg))
				return (1);
			if (!ft_strcmp(arg->arg, "<"))
				check = ft_redir_add(node, REDIR_IN, next->arg);
			else if (!ft_strcmp(arg->arg, ">"))
				check = ft_redir_add(node, REDIR_OUT, next->arg);
			else if (!ft_strcmp(arg->arg, ">>"))
				check = ft_redir_add(node, REDIR_APPEND, next->arg);
			else
				check = ft_redir_add(node, REDIR_HEREDOC, next->arg);
		}
		else
			node->args[(*i)++] = arg;
		if (check)
			return (1);
	}
	return (0);
}

/**
 * @brief Parses the input line into a command structure.
 *
 * This function takes a line of input, counts the number of arguments,
 * allocates memory for the command structure, and extracts each argument.
 * It also handles redirections like input/output files.
 * @param node Pointer to the command structure to fill.
 * @param line The input line to parse.
 * @return int Returns 0 on success, 1 on error (e.g., memory allocation
 * failure).
 */
int	parse_input(t_cmd *node, const char *line)
{
	int		argc;
	int		i;
	char	**new_command;

	node->commands = NULL;
	node->args = NULL;
	if (!line || !*line)
		return (1);
	argc = count_args(line);
	node->args = ft_gcmalloc(CAT_ARGS, sizeof(t_arg *) * (argc + 1));
	if (!node->args)
		return (1);
	i = 0;
	if (ft_fillout_commands(node, line, argc, &i))
		return (1);
	node->args[i] = NULL;
	while (i <= argc)
		node->args[i++] = NULL;
	new_command = ft_expand_wildcards(node->args);
	if (new_command)
		node->commands = new_command;
	return (0);
}
