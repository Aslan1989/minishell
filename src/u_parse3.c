/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:46:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/04 23:23:45 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expands single wildcard
 */
static void	ft_expand_wildcard(t_arg *token, char ***result, int *count)
{
	glob_t	globbuf;
	size_t	i;

	if (token->wildcard == 1 && glob(token->arg, 0, NULL, &globbuf) == 0)
	{
		i = 0;
		while (i < globbuf.gl_pathc)
		{
			*result = (char **)ft_gcrealloc(CAT_ARGS, *result,
					sizeof(char*) * (*count + 2));
			(*result)[*count] = ft_gcstrdup(CAT_ARGS, globbuf.gl_pathv[i]);
			(*count)++;
			i++;
		}
	}
	else
	{
		*result = (char **)ft_gcrealloc(CAT_ARGS, *result,
				sizeof(char*) * (*count + 2));
		(*result)[*count] = ft_gcstrdup(CAT_ARGS, token->arg);
		(*count)++;
	}
	if (token->wildcard == 1)
		globfree(&globbuf);
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

	result = NULL;
	i = 0;
	count = 0;
	while (args[i])
	{
		if (ft_strpbrk(args[i]->arg, "*") == NULL)
		{
			result = (char **)ft_gcrealloc(CAT_ARGS, result,
					sizeof(char*) * (count + 2));
			result[count++] = ft_gcstrdup(CAT_ARGS, args[i]->arg);
		}
		else
			ft_expand_wildcard(args[i], &result, &count);
		i++;
	}
	result[count] = NULL;
	return (result);
}

/**
 * @brief Subfunction to fill out the command structure with arguments.
 */
static int	ft_fillout_commands(t_cmd *node, const char *line, int argc, int *i)
{
	t_arg	*arg;
	int		check;

	*i = 0;
	check = 0;
	while (*line && *i < argc)
	{
		arg = extract_arg(&line);
		// ft_printf("Extracted arg: %s\n", arg->arg);
		if (!arg)
			return (1);
		if (ft_strcmp(arg->arg, "<") == 0)
			check = ft_redir_add(node, REDIR_IN, extract_arg(&line)->arg);
		else if (ft_strcmp(arg->arg, ">") == 0)
			check = ft_redir_add(node, REDIR_OUT, extract_arg(&line)->arg);
		else if (ft_strcmp(arg->arg, ">>") == 0)
			check = ft_redir_add(node, REDIR_APPEND, extract_arg(&line)->arg);
		else if (ft_strcmp(arg->arg, "<<") == 0)
			check = ft_redir_add(node, REDIR_HEREDOC, extract_arg(&line)->arg);
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

	if (!line || !*line)
		return (node->commands = NULL, 1);
	argc = count_args(line);
	node->commands = ft_gcmalloc(CAT_ARGS, sizeof(char *) * (argc + 1));
	node->args = ft_gcmalloc(CAT_ARGS, sizeof(t_arg *) * (argc + 1));
	if (!node->commands || !node->args)
		return (1);
	i = 0;
	// ft_printf("Parsing input: %s\n", line);
	if (ft_fillout_commands(node, line, argc, &i))
		return (1);
	node->args[i] = NULL;
	while (i <= argc)
		node->args[i++] = NULL;
	// ft_expand_env(node->commands);
	node->commands = ft_expand_wildcards(node->args);
	return (0);
}
