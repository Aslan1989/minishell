/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/17 17:51:18 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd *parse_or(t_token **current);
static t_cmd *parse_and(t_token **current);
static t_cmd *parse_pipe(t_token **current);
static t_cmd *parse_word(t_token **current);

// static e_token check(t_token *current, e_token type)
// {
// 	if (!current)
// 		return 0;
// 	return current->type == type;
// }

static t_token *advance(t_token **current)
{
	t_token *prev;

	if (!current || !*current)
		return NULL;
	prev = *current;
	*current = (*current)->next;
	return (prev);
}

// static int is_at_end(t_token *current)
// {
// 	return (!current || current->type == TOK_EOF);
// }

// static t_token *peek(t_token *current)
// {
// 	return current;
// }

static t_token *previous(t_token *current)
{
	if (!current)
		return NULL;
	return current->prev;
}

static int match (t_token **current, e_token type)
{
	if (!current || !*current)
		return (0);
	if ((*current)->type == type)
	{
		*current = (*current)->next;
		return (1);
	}
	return (0);
}

static t_cmd *parse_add_node(t_token *token, t_cmd *next_a, t_cmd *next_b)
{
	t_cmd *node;

	if (!token)
		return (NULL);
	node = ft_gcmalloc(CAT_CMD, sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->token = token;
	// node->value = ft_gcstrdup(CAT_CMD, token->value);
	node->next_a = next_a;
	node->next_b = next_b;
	node->commands = NULL;
	node->path = NULL;
	node->parent = NULL;
	node->pid = 0;
	return (node);
}

t_cmd *parse_or(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	printf ("Parsing OR\n");
	printf ("Current token type: %d\n", (*current)->type);
	expr = parse_and(current);
	while (match(current, TOK_OR))
	{
		operator = previous(*current);
		right = parse_and(current);
		expr = parse_add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd *parse_and(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	printf ("Parsing AND\n");
	printf ("Current token type: %d\n", (*current)->type);
	expr = parse_pipe(current);
	while (match(current, TOK_AND))
	{
		operator = previous(*current);
		right = parse_pipe(current);
		expr = parse_add_node(operator, expr, right);
	}
	return (expr);
}

static t_cmd *parse_pipe(t_token **current)
{
	t_cmd *expr;
	t_cmd *right;
	t_token *operator;

	printf ("Parsing PIPE\n");
	printf ("Current token type: %d\n", (*current)->type);
	expr = parse_word(current);
	while (match(current, TOK_PIPE))
	{
		operator = previous(*current);
		right = parse_word(current);
		expr = parse_add_node(operator, expr, right);
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
	// if (match(current, TOK_WORD))
	// {
	// 	node = parse_add_node(previous(*current), NULL, NULL);
	// 	return node;
	// }
	first = NULL;
	last = NULL;
	while (match(current, TOK_WORD))
	{
		token = previous(*current);
		node = parse_add_node(token, NULL, NULL);
		if (!first)
			first = node;
		else
			last->next_a = node;
		last = node;
	}

	if (first)
		return first;

	if (match(current, TOK_LPAREN))
	{
		node = parse_or(current);
		if (match(current, TOK_RPAREN))
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
