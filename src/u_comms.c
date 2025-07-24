/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_comms.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 11:52:17 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_token(e_token type, const char *value)
{
	t_token	*tok;

	tok = ft_gcmalloc(CAT_TOKEN, sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->type = type;
	tok->value = ft_gcstrdup(CAT_TOKEN, (char *)value);
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}

static int	add_token(t_token **head, e_token type, const char *value, int inc)
{
	t_token	*last;
	t_token	*new_tok;

	new_tok = new_token(type, value);
	if (!new_tok)
		return (inc);
	if (!*head)
	{
		*head = new_tok;
		return (inc);
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_tok;
	new_tok->prev = last;
	return (inc);
}

static void	ft_add_word(char **line, t_token **head)
{
	char	*start;
	char	quote;

	start = *line;
	quote = 0;
	while (**line)
	{
		if ((**line == '"' || **line == '\'') && !quote)
			quote = **line;
		else if (**line == quote && quote)
			quote = 0;
		(*line)++;
		if (**line == '|' || **line == '&' || **line == '(' || **line == ')')
			break ;
	}
	add_token(head, TOK_WORD, ft_gcstrndup(CAT_TOKEN, start, *line - start), 1);
}

static t_token	*ft_tokenize(char *line)
{
	t_token	*head;

	head = NULL;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (!*line)
			break ;
		if (line[0] == '&' && line[1] == '&')
			line += add_token(&head, TOK_AND, "&&", 2);
		else if (line[0] == '|' && line[1] == '|')
			line += add_token(&head, TOK_OR, "||", 2);
		else if (*line == '|')
			line += add_token(&head, TOK_PIPE, "|", 1);
		else if (*line == '(')
			line += add_token(&head, TOK_LPAREN, "(", 1);
		else if (*line == ')')
			line += add_token(&head, TOK_RPAREN, ")", 1);
		else
			ft_add_word(&line, &head);
	}
	add_token(&head, TOK_EOF, "EOF", 1);
	return (head);
}

// void	ft_print_tokens_nested(t_cmd *comms, int depth, int branch)
// {
// 	t_cmd *current;
// 	int i;

// 	current = comms;
// 	if (!current)
// 		return ;
// 	printf(COLOR_G);
// 	i = 0;
// 	while (i++ < depth - 1)
// 		printf("..");
// 	if (depth > 0 && branch == 0)
// 		printf("-");
// 	if (depth > 0 && branch == 1)
// 		printf("=");
// 	printf("%s\n", current->token->value);
// 	ft_print_tokens_nested(current->next_a, depth + 1, 0);
// 	ft_print_tokens_nested(current->next_b, depth + 1, 1);
// 	printf(COLOR_X);
// }

// void	ft_print_token_list(t_token *head)
// {
// 	t_token	*current;

// 	current = head;
// 	while (current)
// 	{
// 		printf(COLOR_Y"%s\n"COLOR_X, current->value);
// 		current = current->next;
// 	}
// }

void	ft_generate_commands(char *line, t_cmd **comms)
{
	t_token	*tokens;
	t_cmd	*ast;

	tokens = ft_tokenize(line);
	if (!tokens)
		return ;
	ast = ft_parse_tokens(&tokens);
	*comms = ast;
}
