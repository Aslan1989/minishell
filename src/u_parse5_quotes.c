/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse5_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/17 19:59:03 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Consume a plain word segment (no quotes) and expand env vars in it.
 */
static char	*open_quote_word(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg;
	while (**arg && **arg != '"' && **arg != '\'')
		(*arg)++;
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	ret = ft_expand_env(ret);
	return (ret);
}

/**
 * @brief Consume a single-quoted segment (no expansions inside).
 */
static char	*open_quote_single(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '\'')
		(*arg)++;
	if (*arg == start)
		return (NULL);
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	if (**arg == '\'')
		(*arg)++;
	return (ret);
}

/**
 * @brief Consume a double-quoted segment and expand env vars inside.
 */
static char	*open_quote_double(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '"')
		(*arg)++;
	if (*arg == start)
		return (NULL);
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	ret = ft_expand_env(ret);
	if (**arg == '"')
		(*arg)++;
	return (ret);
}

/* Return next piece (unquoted/expanded) and advance *line.
 * If parsing produced nothing (e.g., empty quotes),
 advance by 1 char and return NULL. */
static char	*next_piece(char **line)
{
	char	*sub;

	sub = NULL;
	if (**line == '"')
		sub = open_quote_double(line);
	else if (**line == '\'')
		sub = open_quote_single(line);
	else
		sub = open_quote_word(line);
	if (!sub)
	{
		(*line)++;
		return (NULL);
	}
	return (sub);
}

/**
 * @brief Rebuild `arg->arg` concatenating pieces, performing $expansion.
 * Also sets `arg->wildcard = 0` if the resulting text originates from inside
 * quotes and contains wildcard characters (POSIX: quoted globs don't expand).
 */
void	ft_open_quotes(t_arg *arg)
{
	char	*new_arg;
	char	*piece;
	char	*line;
	int		quoted;

	if (!arg || !arg->arg)
		return ;
	arg->wildcard = 1;
	new_arg = NULL;
	line = arg->arg;
	while (*line)
	{
		quoted = (*line == '"' || *line == '\'');
		piece = next_piece(&line);
		if (!piece)
			continue ;
		if (quoted && ft_strpbrk(piece, "*?[]"))
			arg->wildcard = 0;
		new_arg = ft_gcstrjoin(CAT_ARGS, new_arg, piece);
	}
	ft_gcfree(CAT_ARGS, arg->arg);
	arg->arg = new_arg;
}
