/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:53:10 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/11 13:01:49 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the line has unclosed quotes.
 * Returns 1 for single quotes, 2 for double quotes,
 * or 0 if all quotes are closed.
 */
static int	ft_has_unclosed_quotes(const char *line)
{
	int		i;
	int		quotes_s;
	int		quotes_d;

	i = 0;
	quotes_s = 0;
	quotes_d = 0;
	while (line[i])
	{
		if (line[i] == '\'' && quotes_d == 0)
			quotes_s = !quotes_s;
		if (line[i] == '"' && quotes_s == 0)
			quotes_d = !quotes_d;
		i++;
	}
	if (quotes_d)
		return (2);
	if (quotes_s)
		return (1);
	return (0);
}

/**
 * @brief Reads a line from the user in interactive mode.
 *
 * This function displays a custom prompt and waits for user input.
 * It handles multi-line input by checking for unclosed quotes.
 *
 * @return char* The complete user input. Must be freed manually.
 */
static char	*read_full_line_int(void)
{
	char	*line;
	char	*next_line;
	char	*tmp;
	char	*full;

	line = readline(COLOR_Y COLOR_BOLD"minishell$ "COLOR_X);
	while (line && ft_has_unclosed_quotes(line))
	{
		next_line = readline(COLOR_Y COLOR_BOLD"> "COLOR_X);
		if (!next_line)
			break ;
		tmp = ft_strjoin(line, "\n");
		full = ft_strjoin(tmp, next_line);
		free(tmp);
		free(line);
		free(next_line);
		line = full;
	}
	return (line);
}

/**
 * @brief Reads a line from the user in non-interactive mode.
 *
 * This function reads a single line from standard input.
 * It does not handle multi-line input or prompts.
 */
static char	*read_full_line(void)
{
	char	*line;
	int		quotes;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	quotes = ft_has_unclosed_quotes(line);
	if (quotes == 1 || quotes == 2)
	{
		free(line);
		if (quotes == 2)
			ft_print_error("minishell: unexpected \
EOF while looking for matching `\"'\n");
		else
			ft_print_error("minishell: unexpected \
EOF while looking for matching `\''\n");
		return (NULL);
	}
	return (line);
}

/**
 * @brief Reads a full prompt from the user.
 *
 * This function checks if the shell is interactive.
 * If it is, it reads a full line with prompts and multi-line support.
 * If not, it reads a single line without prompts.
 */
char	*read_full_prompt(void)
{
	t_shell	*shell;

	shell = get_shell();
	if (shell->is_interactive)
		return (read_full_line_int());
	return (read_full_line());
}
