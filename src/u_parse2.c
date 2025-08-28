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

static const char	*curr_tok_value(t_token **current)
{
	if (current && *current)
		return ((*current)->value);
	return (NULL);
}

/**
 * @brief Parse OR chain: and_expr ( '||' and_expr )*
 */
t_cmd	*parse_or(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	if (ft_p_check(*current, TOK_OR))
		return (parser_syntax_error((*current)->value), NULL);
	expr = parse_and(current);
	while (ft_p_check(*current, TOK_OR))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_and(current);
		if (!right)
		{
			parser_syntax_error(curr_tok_value(current));
			return (expr);
		}
		if (!expr)
		{
			parser_syntax_error(operator->value);
			return (NULL);
		}
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

/**
 * @brief Parse AND chain: pipe_expr ( '&&' pipe_expr )*
 */
t_cmd	*parse_and(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	if (ft_p_check(*current, TOK_AND))
		return (parser_syntax_error((*current)->value), NULL);
	expr = parse_pipe(current);
	while (ft_p_check(*current, TOK_AND))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_pipe(current);
		if (!right)
		{
			parser_syntax_error(curr_tok_value(current));
			return (expr);
		}
		if (!expr)
		{
			parser_syntax_error(operator->value);
			return (NULL);
		}
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

/**
 * @brief Parse pipeline: word ( '|' word )*
 */
t_cmd	*parse_pipe(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	if (ft_p_check(*current, TOK_PIPE))
		return (parser_syntax_error((*current)->value), NULL);
	expr = parse_word(current);
	while (ft_p_check(*current, TOK_PIPE))
	{
		operator = (*current);
		ft_p_advance(current);
		right = parse_word(current);
		if (!expr)
		{
			parser_syntax_error(operator->value);
			return (NULL);
		}
		if (!right)
		{
			parser_syntax_error(curr_tok_value(current));
			return (expr);
		}
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

/**
 * @brief Parse a command word or a parenthesized subexpression.
 */
t_cmd	*parse_word(t_token **current)
{
	t_cmd	*node;
	t_token	*paren_token;

	if (!current || !*current)
		return (NULL);
	if (ft_p_check(*current, TOK_RPAREN))
		return (parser_syntax_error((*current)->value), NULL);
	node = parse_word_sub(current);
	if (node)
		return (node);
	if (ft_p_check(*current, TOK_LPAREN))
	{
		paren_token = (*current);
		ft_p_advance(current);
		node = parse_or(current);
		if (!node && *current && ft_p_check(*current, TOK_RPAREN))
			return (parser_syntax_error((*current)->value), NULL);
		if (!*current || !ft_p_check(*current, TOK_RPAREN))
			return (parser_syntax_error(NULL), NULL);
		ft_p_advance(current);
		node = ft_p_add_node(paren_token, node, NULL);
		return (node);
	}
	return (NULL);
}
