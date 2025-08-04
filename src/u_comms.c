/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_comms.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/04 15:13:04 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_token(t_etoken type, const char *value)
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

static int	add_token(t_token **head, t_etoken type, const char *value)
{
	t_token	*last;
	t_token	*new_tok;
	int		len;

	len = ft_strlen(value);
	new_tok = new_token(type, value);
	if (!new_tok)
		return (len);
	if (!*head)
	{
		*head = new_tok;
		return (len);
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_tok;
	new_tok->prev = last;
	return (len);
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
	add_token(head, TOK_WORD, ft_gcstrndup(CAT_TOKEN, start, *line - start));
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
			line += add_token(&head, TOK_AND, "&&");
		else if (line[0] == '|' && line[1] == '|')
			line += add_token(&head, TOK_OR, "||");
		else if (*line == '|')
			line += add_token(&head, TOK_PIPE, "|");
		else if (*line == '(')
			line += add_token(&head, TOK_LPAREN, "(");
		else if (*line == ')')
			line += add_token(&head, TOK_RPAREN, ")");
		else
			ft_add_word(&line, &head);
	}
	add_token(&head, TOK_EOF, "EOF");
	return (head);
}

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
