/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse5_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 14:59:40 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	{
		ret = ft_gcstrdup(CAT_ARGS, "");
		if (**arg == '"')
			(*arg)++;
		return (ret);
	}
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

static void	qctx_append_piece(t_arg *arg, t_quote_ctx *c)
{
	char	*piece;
	int		starts_quoted;

	starts_quoted = (*c->line == '"' || *c->line == '\'');
	piece = next_piece(&c->line);
	if (!piece)
		return ;
	if (starts_quoted)
	{
		c->saw_q = 1;
		if (ft_strpbrk(piece, "*?[]"))
			arg->wildcard = 0;
	}
	else
		c->saw_u = 1;
	c->new_arg = ft_gcstrjoin(CAT_ARGS, c->new_arg, piece);
}

/**
 * @brief Rebuild `arg->arg` concatenating pieces, performing $expansion.
 * Also sets `arg->wildcard = 0` if the resulting text originates from inside
 * quotes and contains wildcard characters (POSIX: quoted globs don't expand).
 */
void	ft_open_quotes(t_arg *arg)
{
	t_quote_ctx	c;

	if (!arg || !arg->arg)
		return ;
	arg->wildcard = 1;
	arg->quoted = 0;
	c.new_arg = NULL;
	c.line = arg->arg;
	c.saw_q = 0;
	c.saw_u = 0;
	while (*c.line)
		qctx_append_piece(arg, &c);
	ft_gcfree(CAT_ARGS, arg->arg);
	if (!c.new_arg)
		c.new_arg = ft_gcstrdup(CAT_ARGS, "");
	arg->arg = c.new_arg;
	if (c.saw_q && !c.saw_u)
		arg->quoted = 1;
}
