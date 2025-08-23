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

/**
 * @brief Append a token at the tail of a doubly-linked token list.
 *
 * Allocates the token node and duplicates `value` into CAT_TOKEN.
 * Returns the number of characters consumed from input (length of value),
 * which the caller adds to its pointer.
 *
 * @param head Pointer to the head of token list (updated on first insert).
 * @param type Token kind.
 * @param value Lexeme string.
 * @return int Number of chars consumed (len(value)), or len even on alloc
 *         error to let the caller progress; head will remain unchanged.
 */
static int	add_token(t_token **head, t_etoken type, const char *value)
{
	t_token	*last;
	t_token	*new_tok;
	int		len;

	len = ft_strlen(value);
	new_tok = ft_gcmalloc(CAT_TOKEN, sizeof(t_token));
	if (!new_tok)
		return (len);
	new_tok->type = type;
	new_tok->value = ft_gcstrdup(CAT_TOKEN, value);
	new_tok->next = NULL;
	new_tok->prev = NULL;
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

/**
 * @brief Consume a WORD token possibly spanning quotes (no expansion yet).
 *
 * Reads until a metacharacter (| & ( ) ;) not inside quotes.
 *
 * @param line Address of scanning pointer.
 * @param head Tokens list head.
 */
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
		if (!quote && (**line == '|' || **line == '&' || **line == '(' \
		|| **line == ')'))
			break ;
	}
	add_token(head, TOK_WORD, ft_gcstrndup(CAT_TOKEN, start, *line - start));
}

static int	try_add_operator(char **line, t_token **head)
{
	if ((*line)[0] == '&' && (*line)[1] == '&')
		return (*line += add_token(head, TOK_AND, "&&"), 1);
	if ((*line)[0] == '|' && (*line)[1] == '|')
		return (*line += add_token(head, TOK_OR, "||"), 1);
	if (**line == '|')
		return (*line += add_token(head, TOK_PIPE, "|"), 1);
	if (**line == '(')
		return (*line += add_token(head, TOK_LPAREN, "("), 1);
	if (**line == ')')
		return (*line += add_token(head, TOK_RPAREN, ")"), 1);
	return (0);
}

/**
 * @brief Lexical analysis: convert raw input `line` to token list.
 * @param line Input string (owned by caller).
 * @return t_token* Head of tokens list (terminated by TOK_EOF), or NULL.
 */
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
		if (!try_add_operator(&line, &head))
			ft_add_word(&line, &head);
	}
	add_token(&head, TOK_EOF, "EOF");
	return (head);
}

/**
 * @brief Top-level entry: tokenize then parse to AST.
 * @param line Raw command line (may embed quotes etc.).
 * @param comms Out parameter: receives AST root.
 */
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
