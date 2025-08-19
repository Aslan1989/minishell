/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 14:17:51 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if current token matches expected type.
 * @param current Current token pointer.
 * @param type Expected type.
 * @return t_etoken 1 if match, 0 otherwise.
 */
t_etoken	ft_p_check(t_token *current, t_etoken type)
{
	if (!current)
		return (0);
	return (current->type == type);
}

/**
 * @brief Return current then advance pointer.
 * @param current Pointer to current token.
 * @return t_token* Previous (consumed) token.
 */
t_token	*ft_p_advance(t_token **current)
{
	t_token	*prev;

	if (!current || !*current)
		return (NULL);
	prev = *current;
	*current = (*current)->next;
	return (prev);
}

/**
 * @brief Allocate and connect an AST node for `token` with its children.
 * @param token The token that determines node type/value.
 * @param next_a Left/first child.
 * @param next_b Right/second child.
 * @return t_cmd* Newly allocated node (CAT_CMD) or NULL.
 */
t_cmd	*ft_p_add_node(t_token *token, t_cmd *next_a, t_cmd *next_b)
{
	t_cmd	*node;

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

/**
 * @brief Parse entry point: sequence (handles ';').
 * @param tokens Pointer to token stream.
 * @return t_cmd* AST root.
 */
t_cmd	*ft_parse_tokens(t_token **tokens)
{
	t_cmd	*ast;
	t_token	*current;

	current = *tokens;
	if (current && current->type == TOK_SEMI)
	{
		parser_syntax_error(current->value);
		return (NULL);
	}
	ast = parse_or(&current);
	if (!ast && g_syntax_error)
		return (NULL);
	if (current && current->type != TOK_EOF)
	{
		parser_syntax_error(current->value);
		return (NULL);
	}
	return (ast);
}

/**
 * @brief Helper: consume one or more WORD tokens into a simple chain.
 */
t_cmd	*parse_word_sub(t_token **current)
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
