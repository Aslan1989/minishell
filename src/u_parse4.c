/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/04 23:17:41 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the number of arguments in the input line.
 *
 * Handles spaces and quotes. For example:
 *  - Input: echo "hello world"
 *  - Output: 2 arguments: "echo", "hello world"
 *
 * @param line Input string from user.
 * @return int Number of separate arguments.
 */
// int	count_args(const char *line)
// {
// 	int	count;
// 	int	in_word;
// 	int	quote;
// 	int	i;

// 	count = 0;
// 	quote = 0;
// 	in_word = 0;
// 	i = 0;
// 	while (line[i])
// 	{
// 		if ((line[i] == '\'' || line[i] == '"') && !quote)
// 			quote = line[i];
// 		else if (line[i] == quote && i > 1 && (line[i - 1] != '\\'))
// 			quote = 0;
// 		else if (!quote && (line[i] == ' ' || line[i] == '\t' || line[i] == '>'
// 				|| line[i] == '<' || line[i] == '$'))
// 			in_word = 0;
// 		else if (!in_word)
// 		{
// 			in_word = 1;
// 			count++;
// 		}
// 		i++;
// 	}
// 	return (count);
// }

int	count_args(const char *line)
{
	int	count;
	int	in_word;
	int	quote;
	int	i;

	count = 0;
	quote = 0;
	in_word = 0;
	i = -1;
	while (line[++i])
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote && i > 1 && (line[i - 1] != '\\'))
			quote = 0;
		else if (!quote && (line[i] == ' ' || line[i] == '\t' || line[i] == '>'
				|| line[i] == '<' || line[i] == '$'))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
	}
	return (count);
}

static int	ft_check_redir(const char **line)
{
	if (!(**line))
		return (0);
	if ((**line) == '<')
	{
		(*line)++;
		if (**line == '<')
			return ((*line)++, 2);
		return (1);
	}
	else if ((**line) == '>')
	{
		(*line)++;
		if (**line == '>')
			return ((*line)++, 2);
		return (1);
	}
	return (0);
}

static int	calc_arg_len(const char **line)
{
	int		len;
	int		in_quote;
	char	quote;

	len = ft_check_redir(line);
	in_quote = 0;
	quote = 0;
	if (len > 0)
		return (len);
	while (**line && (!ft_isnotaword(**line) || in_quote))
	{
		if ((**line == '"' || **line == '\'') && !in_quote)
		{
			in_quote = 1;
			quote = **line;
		}
		else if (**line == quote && in_quote)
		{
			in_quote = 0;
			quote = 0;
		}
		len++;
		(*line)++;
	}
	return (len);
}

/**
 * @brief Extracts a single argument from the input line, supporting quotes.
 *
 * This function takes a pointer to a C string and extracts the next argument,
 * handling single and double quotes so that quoted strings are preserved
 * as one argument even if they contain spaces.
 *
 * @param line Pointer to the current position in the input line.
 * The pointer is updated.
 * @return char* The newly allocated string containing the argument
 * (unquoted), or NULL on error.
 */
t_arg	*extract_arg(const char **line)
{
	const char	*start;
	int			len;
	t_arg		*arg;

	while (ft_isspace(**line))
		(*line)++;
	start = *line;
	len = calc_arg_len(line);
	if (len <= 0)
		return (NULL);
	arg = ft_gcmalloc(CAT_ARGS, sizeof(t_arg));
	if (!arg)
		return (NULL);
	arg->arg = ft_gcstrndup(CAT_ARGS, (char *)start, len);
	ft_open_quotes(arg);
	return (arg);
}
