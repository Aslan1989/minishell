/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:48:05 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/23 17:12:24 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_redir_check_next(char *next_token)
{
	if (!next_token || !*next_token)
	{
		ft_print_error("minishell: syntax error near unexpected token `newline'");
		return (1);
	}
	if (ft_strcmp(next_token, "<") == 0
		|| ft_strcmp(next_token, ">") == 0
		|| ft_strcmp(next_token, ">>") == 0
		|| ft_strcmp(next_token, "<<") == 0)
	{
		ft_print_error("minishell: syntax error near unexpected token `<\
		'");
		ft_print_error(next_token);
		ft_print_error("'\n");
		return (1);
	}
	return (0);
}

int	ft_here_doc_input(char *limiter)
{
	int		pipe_fd[2];
	char	*line;

	line = NULL;
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		exit(1);
	}
	ft_printf(COLOR_Y "> " COLOR_X);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		ft_printf(COLOR_Y "> " COLOR_X);
		free(line);
		line = get_next_line(0);
	}
	if (line)
		free(line);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	ft_redir_add(t_cmd *cmd, e_redir type, char *next_token)
{
	t_redir *new_redir;
	t_redir *last;

	if (!cmd || ft_redir_check_next(next_token))
		return (1);
	new_redir = ft_gcmalloc(CAT_CMD, sizeof(t_redir));
	if (!new_redir)
		return (1);
	if (type == REDIR_HEREDOC)
		new_redir->fd = ft_here_doc_input(next_token);
	new_redir->type = type;
	new_redir->value = ft_gcstrdup(CAT_CMD, next_token);
	new_redir->next = NULL;
	if (!cmd->redir)
	{
		cmd->redir = new_redir;
	}
	else
	{
		last = cmd->redir;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	return (0);
}
