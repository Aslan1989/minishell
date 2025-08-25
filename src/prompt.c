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
 * @brief Check if a line contains unclosed quotes.
 *
 * - Tracks single quotes (') and double quotes (").
 * - A quote is only toggled if the other type of quote is not currently open.
 * - Returns 1 if single quotes remain open, 2 if double quotes remain open,
 *  0 otherwise.
 * Scan through the line
 * Toggle single quotes if not inside double quotes
 * Toggle double quotes if not inside single quotes
 * Double quotes left open
 * Single quotes left open
 * All balanced
 *
 * @param line Input line to check.
 * @return int 0 if no unclosed quotes, 1 if single quotes unclosed,
 * 2 if double quotes unclosed.
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
 * @brief Read a full line interactively (with prompt), handling unclosed quotes.
 *
 * If quotes are left open, prints a secondary prompt ("> ") and keeps reading
 * until all quotes are closed or EOF is reached.
 *
 * @return char* Dynamically allocated line string, or NULL on EOF/error.
 * Current input line
 * Continuation if quotes are unclosed
 * Initial prompt
 * While quotes are open
 * Secondary prompt
 * EOF (Ctrl-D)
 * Add newline between parts
 * Concatenate the two parts
 * Free temp buffer
 * Continue with new combined line
 *
 * @return char* Dynamically allocated line string, or NULL on EOF/error.
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
 * @brief Read a full line in non-interactive mode (from stdin).
 *
 * Uses get_next_line() to read, strips the final newline, and checks for
 * unclosed quotes. If quotes are left open, prints an error and returns NULL.
 *
 * @return char* Line string without trailing newline, or NULL on EOF/error.
 * Read one line from stdin
 * EOF
 * Strip trailing newline
 * Check for open quotes
 * If any are unclosed
 * Abort line
 *
 * @return char* Line string without trailing newline, or NULL on EOF/error.
 */
static char	*read_full_line(void)
{
	char	*line;
	int		quotes;

	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	line[ft_strlen(line) - 1] = '\0';
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
 * @brief Read input line depending on shell mode
 * (interactive or non-interactive).
 *
 * - Interactive: use readline() with prompt and handle multi-line quotes.
 * - Non-interactive: use get_next_line() from stdin.
 * 
 * @return char* Allocated line, or NULL on EOF/error.
 * Access global shell state
 * Choose correct input function
 *
 * @return char* Allocated line, or NULL on EOF/error.
 */
char	*read_full_prompt(void)
{
	t_shell	*shell;

	shell = get_shell();
	if (shell->is_interactive)
		return (read_full_line_int());
	return (read_full_line());
}
