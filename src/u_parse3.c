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

int	has_wc_token(t_arg *a)
{
	if (!a || !a->wildcard)
		return (0);
	if (ft_strpbrk(a->arg, "*?["))
		return (1);
	return (0);
}

char	**ft_expand_wildcards(t_arg **args)
{
	char	**res;
	int		cnt;
	int		i;
	int		matched;
	int		has_slash;

	res = NULL;
	cnt = 0;
	i = 0;
	while (args && args[i])
	{
		has_slash = (ft_strchr(args[i]->arg, '/') != NULL);
		if (has_wc_token(args[i]) && !has_slash)
		{
			matched = expand_one_pattern(args[i]->arg, &res, &cnt);
			if (!matched && append_result(&res, &cnt, args[i]->arg))
				return (res);
		}
		else if (append_result(&res, &cnt, args[i]->arg))
			return (res);
		i++;
	}
	return (res);
}

static int	handle_redirection(t_cmd *node, t_arg *arg, const char **line)
{
	t_arg	*next;
	int		check;

	next = extract_arg(line);
	if (!next)
	{
		parser_syntax_error(NULL);
		return (1);
	}
	if (ft_redir_check_next(next->arg))
		return (1);
	if (!ft_strcmp(arg->arg, "<"))
		check = ft_redir_add(node, REDIR_IN, next->arg);
	else if (!ft_strcmp(arg->arg, ">"))
		check = ft_redir_add(node, REDIR_OUT, next->arg);
	else if (!ft_strcmp(arg->arg, ">>"))
		check = ft_redir_add(node, REDIR_APPEND, next->arg);
	else if (!ft_strcmp(arg->arg, "<>"))
		check = ft_redir_add(node, REDIR_INOUT, next->arg);
	else
		check = ft_redir_add(node, REDIR_HEREDOC, next->arg);
	return (check);
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
	int		cap;

	*i = 0;
	cap = argc;
	if (cap < 1)
		cap = 1;
	while (*line)
	{
		skip_ws(&line);
		if (!*line)
			break ;
		arg = extract_arg(&line);
		if (!arg)
			return (1);
		if (is_redir_word(arg))
		{
			if (handle_redirection(node, arg, &line))
				return (1);
		}
		else if (append_arg(node, arg, i, &cap))
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
	new_command = ft_expand_wildcards(node->args);
	if (new_command)
		node->commands = new_command;
	return (0);
}
