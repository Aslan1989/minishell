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
 * @brief Add a new token node to the end of the token list.
 *
 * Creates a new token with given type and string value, then appends it
 * to the doubly linked list of tokens. Returns the length of the token value.
 *
 * If list empty → this is first
 * Else go to last node
 * Link new token at end
 * @param head  Pointer to the head of the token list.
 * @param type  Token type (enum t_etoken).
 * @param value String value for this token.
 * @return int  Length of the token string.
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
 * @brief Parse and add a "word" token (command or argument).
 *
 * A word can be a sequence of chars, possibly quoted. The function moves
 * the line pointer until it reaches a special operator or space.
 * Start of the word
 * Loop until operator or space
 * Opening quote
 * Closing quote
 * Stop if we hit operator outside quotes
 * Save substring as TOK_WORD
 *
 * @param line  Pointer to current input pointer (will advance).
 * @param head  Token list head pointer (new token is appended here).
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
		if (!quote && (**line == '|' || **line == '&' || **line == '('
				|| **line == ')'))
			break ;
	}
	add_token(head, TOK_WORD, ft_gcstrndup(CAT_TOKEN, start, *line - start));
}

/**
 * @brief Try to detect and add an operator token (&&, ||, |, (, )).
 *
 * If a match is found, adds the token, moves the line pointer, and returns 1.
 * If no operator matches, returns 0 (so caller can parse as word).
 *
 * @param line Current input pointer (will advance if operator found).
 * @param head Token list head pointer.
 * @return int 1 if operator was found and added, 0 otherwise.
 */
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
 * @brief Tokenize a command line string into a linked list of tokens.
 *
 * Skips whitespace, then either:
 *  - Detects an operator with try_add_operator().
 *  - Or parses a word with ft_add_word().
 * At the end, always appends an EOF token.
 * Scan until end of line
 * Skip whitespace
 * End reached
 * Try operator, else parse as word
 * Always terminate with EOF token
 * @param line Input string (modified pointer).
 * @return t_token* Head of the token linked list.
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
 * @brief Parse a line into an AST (Abstract Syntax Tree).
 *
 * Step 1: Tokenize line into linked list.
 * Step 2: Call parser (ft_parse_tokens) to build AST.
 * Step 3: Store AST pointer in comms.
 *
 * Lexical analysis (split into tokens)
 * On error, just return NULL
 * Build AST from token list
 * Return to caller
 * @param line  Raw command line input.
 * @param comms Output: pointer to root of AST.
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
