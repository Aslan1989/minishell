/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:48:05 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 14:19:00 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the next token is a valid redirection target.
 *
 * This function checks if the next token is a valid file name or
 * redirection operator. If not, it prints an error message and returns 1.
 *
 * @param next_token The next token to check.
 * @return int 0 if valid, 1 if invalid.
 */
int	ft_redir_check_next(char *next_token)
{
	if (!next_token || !*next_token)
	{
		ft_print_error("minishell: syntax error near \
				unexpected token `newline'");
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

/**
 * @brief Handles input for a heredoc redirection.
 *
 * This function reads lines from standard input until it encounters
 * a line that matches the specified delimiter. It writes the input to a
 * pipe, which can be used as input for a command.
 *
 * @param limiter The delimiter to stop reading input.
 * @return int The file descriptor of the pipe that contains the heredoc input.
 */
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

/**
 * @brief Adds a redirection to the command's redirection list.
 *
 * This function creates a new redirection node and adds it to the end of the
 * command's redirection list. It handles different types of redirections
 * (input, output, append, heredoc).
 *
 * @param cmd The command structure to which the redirection will be added.
 * @param type The type of redirection (REDIR_IN, REDIR_OUT,
 * REDIR_APPEND, REDIR_HEREDOC).
 * @param next_token The token value for the redirection
 * (e.g., file name / limiter).
 * @return int 0 on success, 1 on error.
 */
int	ft_redir_add(t_cmd *cmd, t_eredir type, char *next_token)
{
	t_redir	*new_redir;
	t_redir	*last;

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
