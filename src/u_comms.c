/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_comms.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/16 17:08:21 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*new_token(e_token type, const char *value) {
	t_cmd *tok = ft_gcmalloc(CAT_TOKEN, sizeof(t_cmd));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = ft_gcstrdup(CAT_TOKEN, (char *)value);
	tok->next_a = NULL;
	tok->next_b = NULL;
	tok->commands = NULL;
	tok->path = NULL;
	tok->parent = NULL;
	tok->depth = 0;
	return (tok);
}

static t_cmd	*add_token(t_cmd **head, e_token type, const char *value)
{
	t_cmd *last;
	t_cmd *new_tok;

	new_tok = new_token(type, value);
	if (!new_tok)
		return (NULL);
	if (!*head)
	{
		*head = new_tok;
		return (new_tok);
	}
	last = *head;
	while (last->next_a)
		last = last->next_a;
	last->next_a = new_tok;
	new_tok->parent = last; // Set parent to the last token
	return (new_tok);
}

static t_cmd	*ft_tokenize(char *line)
{
	t_cmd	*head;
	char	*start;

	head = NULL;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (!*line)
			break;
		if (line[0] == '&' && line[1] == '&')
		{
			add_token(&head, TOK_AND, "&&");
			line += 2;
		} else if (line[0] == '|' && line[1] == '|')
		{
			add_token(&head, TOK_OR, "||");
			line += 2;
		} else if (*line == '|') {
			add_token(&head, TOK_PIPE, "|");
			line++;
		} else if (*line == '(') {
			add_token(&head, TOK_LPAREN, "(");
			line++;
		} else if (*line == ')') {
			add_token(&head, TOK_RPAREN, ")");
			line++;
		} else {
			start = line;
			while (*line && !ft_isspace(*line) && !strchr("|&()", *line))
				line++;
			add_token(&head, TOK_WORD, ft_gcstrndup(CAT_TOKEN, start, line - start));
		}
	}
	add_token(&head, TOK_EOF, "EOF");
	return (head);
}

// void ft_print_tokens(t_cmd **comms)
// {
// 	t_cmd *current = *comms;
// 	while (current)
// 	{
// 		printf(COLOR_G" %d %s %d\n"COLOR_X, current->depth, current->value, current->type);
// 		current = current->next_a;
// 	}
// }

// static void	ft_comms_set_depth(t_cmd **comms)
// {
// 	t_cmd	*current = *comms;
// 	int		depth = 0;

// 	while (current)
// 	{
// 		if (current->type == TOK_LPAREN)
// 		{
// 			depth++;
// 			current->depth = depth;
// 		}
// 		else if (current->type == TOK_RPAREN)
// 		{
// 			current->depth = depth;
// 			depth--;
// 			if (depth < 0)
// 				break;
// 		}
// 		else
// 			current->depth = depth;
// 		current = current->next_a;
// 	}
// 	if (depth != 0)
// 		fprintf(stderr, "Error: Unmatched parentheses at end of command\n");
// }

t_cmd	*ft_comms_sort_parenth(t_cmd *current, int depth)
{
	t_cmd	*start;

	start = current;
	(void)depth;
	while (current)
	{
		if (current->type == TOK_LPAREN)
		{
			current->next_b = ft_comms_sort_parenth(current->next_a, current->depth);
			current->next_b->parent = current;
			current = current->next_b;
		}
		else if (current->type == TOK_RPAREN)
		{
			if (current->parent->next_a == current)
				current->parent->next_a = NULL;
			if (current->parent->next_b == current)
				current->parent->next_b = NULL;
			return (current->next_a);
		}
		else
			current = current->next_a;
	}
	return (start);
}

void	ft_print_tokens_nested(t_cmd *comms, int depth, int branch)
{
	t_cmd *current;
	int i;

	current = comms;
	if (!current)
		return ;
	i = 0;
	while (i++ < depth - 1)
		printf("  ");
	if (depth > 0 && branch == 0)
		printf("-");
	if (depth > 0 && branch == 1)
		printf("=");
	printf("%s\n", current->value);
	ft_print_tokens_nested(current->next_a, depth + 1, 0);
	ft_print_tokens_nested(current->next_b, depth + 1, 1);
}

void	ft_generate_commands(char *line, t_cmd **comms)
{
	t_cmd *temp;
	//ft_free_comms(comms);
	*comms = ft_tokenize(line);
	if (!*comms)
		return ;
	//ft_comms_set_depth(comms);
	temp = *comms;
	(void)temp;
	*comms = ft_comms_sort_parenth(*comms, 0);
	//ft_print_tokens(comms);
	ft_print_tokens_nested(*comms, 0, 0);
	//ft_restructure_commands(comms);
}
