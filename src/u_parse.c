/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 11:27:16 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

e_token	ft_p_check(t_token *current, e_token type)
{
	if (!current)
		return (0);
	return (current->type == type);
}

t_token	*ft_p_advance(t_token **current)
{
	t_token	*prev;

	if (!current || !*current)
		return (NULL);
	prev = *current;
	*current = (*current)->next;
	return (prev);
}

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

t_cmd	*ft_parse_tokens(t_token **tokens)
{
	t_cmd	*ast;
	t_token	*current;

	current = *tokens;
	ast = parse_or(&current);
	return (ast);
}
