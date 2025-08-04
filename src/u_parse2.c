/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 11:40:18 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_or(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	expr = parse_and(current);
	while (ft_p_check(*current, TOK_OR))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_and(current);
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

t_cmd	*parse_and(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	expr = parse_pipe(current);
	while (ft_p_check(*current, TOK_AND))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_pipe(current);
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

t_cmd	*parse_pipe(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	expr = parse_word(current);
	while (ft_p_check(*current, TOK_PIPE))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_word(current);
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd	*parse_word_sub(t_token **current)
{
	t_cmd	*first;
	t_cmd	*node;
	t_cmd	*last;
	t_token	*token;

	first = NULL;
	last = NULL;
	while (ft_p_check(*current, TOK_WORD))
	{
		token = (*current);
		ft_p_advance(current);
		node = ft_p_add_node(token, NULL, NULL);
		parse_input(node, token->value);
		if (!first)
			first = node;
		else
			last->next_a = node;
		last = node;
	}
	return (first);
}

t_cmd	*parse_word(t_token **current)
{
	t_cmd	*node;

	if (!current || !*current)
		return (NULL);
	node = parse_word_sub(current);
	if (node)
		return (node);
	if (ft_p_check(*current, TOK_LPAREN))
	{
		ft_p_advance(current);
		node = parse_or(current);
		if (ft_p_check(*current, TOK_RPAREN))
			ft_p_advance(current);
		return (node);
	}
	return (NULL);
}
