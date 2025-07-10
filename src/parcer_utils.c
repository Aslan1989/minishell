/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/09 13:24:54 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_quotes(const char **line, int *in_quote, char *quote)
{
	while (*line && (!isspace(**line) || *in_quote))
	{
		if ((**line == '\'' || **line == '"') && !*in_quote)
		{
			*in_quote = 1;
			*quote = **line;
		}
		else if (**line == *quote && *in_quote)
		{
			*in_quote = 0;
			*quote = 0;
		}
		(*line)++;
	}
}

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
int	count_args(const char *line)
{
	int		count;
	int		in_quote;
	char	quote;

	count = 0;
	in_quote = 0;
	quote = 0;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '\0')
			break ;
		count++;
		skip_quotes(&line, &in_quote, &quote);
	}
	return (count);
}

static int	calc_arg_len(const char **line)
{
	int		len;
	int		in_quote;
	char	quote;

	len = 0;
	in_quote = 0;
	quote = 0;
	while (**line && (!ft_isspace(**line) || in_quote))
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
		else
			len++;
		(*line)++;
	}
	return (len);
}

static char	*copy_arg(const char *start, int len, char *arg)
{
	int		i;
	int		in_quote;
	char	quote;

	i = 0;
	in_quote = 0;
	quote = 0;
	while (i < len)
	{
		if ((*start == '"' || *start == '\'') && !in_quote)
		{
			in_quote = 1;
			quote = *start;
		}
		else if (*start == quote && in_quote)
		{
			in_quote = 0;
			quote = 0;
		}
		else
			arg[i++] = *start;
		start++;
	}
	arg[i] = '\0';
	return (arg);
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
char	*extract_arg(const char **line)
{
	const char	*start;
	int			len;
	char		*arg;

	while (ft_isspace(**line))
		(*line)++;
	start = *line;
	len = calc_arg_len(line);
	arg = malloc(len + 1);
	if (!arg)
		return (NULL);
	return (copy_arg(start, len, arg));
}
