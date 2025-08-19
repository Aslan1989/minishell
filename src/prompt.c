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

char	*read_full_prompt(void)
{
	t_shell	*shell;

	shell = get_shell();
	if (shell->is_interactive)
		return (read_full_line_int());
	return (read_full_line());
}
