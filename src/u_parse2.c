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

/**
 * @brief Parse a sequence: expr (';' expr)*
 */
t_cmd	*parse_seq(t_token **current)
{
	t_cmd	*expr;
	t_cmd	*right;
	t_token	*operator;

	expr = parse_or(current);
	while (ft_p_check(*current, TOK_SEMI))
	{
		operator = *current;
		ft_p_advance(current);
		right = parse_or(current);
		expr = ft_p_add_node(operator, expr, right);
	}
	return (expr);
}

/**
 * @brief Parse OR chain: and_expr ( '||' and_expr )*
 */
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

/**
 * @brief Parse AND chain: pipe_expr ( '&&' pipe_expr )*
 */
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

/**
 * @brief Parse pipeline: word ( '|' word )*
 */
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

/**
 * @brief Parse a command word or a parenthesized subexpression.
 */
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
		if (!ft_p_check(*current, TOK_RPAREN))
		{
			ft_print_error("minishell: syntax error: unexpected token\n");
			return (NULL);
		}
		ft_p_advance(current);
		return (node);
	}
	return (NULL);
}
