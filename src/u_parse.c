/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/23 17:11:57 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd *parse_or(t_token **current);
static t_cmd *parse_and(t_token **current);
static t_cmd *parse_pipe(t_token **current);
static t_cmd *parse_word(t_token **current);

static e_token check(t_token *current, e_token type)
{
	if (!current)
		return 0;
	return current->type == type;
}

static t_token *advance(t_token **current)
{
	t_token *prev;

	if (!current || !*current)
		return NULL;
	prev = *current;
	*current = (*current)->next;
	return (prev);
}

static t_cmd *add_node(t_token *token, t_cmd *next_a, t_cmd *next_b)
{
	t_cmd *node;

	if (!token)
		return (NULL);
	node = ft_gcmalloc(CAT_CMD, sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->token = token;
	node->next_a = next_a;
	node->next_b = next_b;
	node->isbuiltin = 0;
	node->commands = NULL;
	node->path = NULL;
	node->parent = NULL;
	node->pid = 0;
	node->redir = NULL;
	node->fd_in = -1;
	node->fd_out = -1;
	return (node);
}

t_cmd *parse_or(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	expr = parse_and(current);
	while (check(*current, TOK_OR))
	{
		operator = (*current);
		advance(current);
		right = parse_and(current);
		expr = add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd *parse_and(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	expr = parse_pipe(current);
	while (check(*current, TOK_AND))
	{
		operator = (*current);
		advance(current);
		right = parse_pipe(current);
		expr = add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd *parse_pipe(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	expr = parse_word(current);
	while (check(*current, TOK_PIPE))
	{
		operator = (*current);
		advance(current);
		right = parse_word(current);
		expr = add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd *parse_word(t_token **current)
{
	t_cmd	*first;
	t_cmd	*node;
	t_cmd	*last;
	t_token	*token;

	if (!current || !*current)
		return NULL;
	first = NULL;
	last = NULL;
	while (check(*current, TOK_WORD))
	{
		token = (*current);
		advance(current);
		node = add_node(token, NULL, NULL);
		parse_input(node, token->value);
		if (!first)
			first = node;
		else
			last->next_a = node;
		last = node;
	}

	if (first)
		return first;

	if (check(*current, TOK_LPAREN))
	{
		advance(current);
		node = parse_or(current);
		if (check(*current, TOK_RPAREN))
			advance(current);
		return node;
	}
	return NULL;
}

t_cmd *ft_parse_tokens(t_token **tokens)
{
	t_cmd	*ast;
	t_token	*current;

	current = *tokens;
	ast = parse_or(&current);
	return (ast);
}
