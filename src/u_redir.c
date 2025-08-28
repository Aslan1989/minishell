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
		parser_syntax_error(NULL);
		return (1);
	}
	if (!ft_strcmp(next_token, "<") || !ft_strcmp(next_token, ">")
		|| !ft_strcmp(next_token, ">>") || !ft_strcmp(next_token, "<<")
		|| !ft_strcmp(next_token, "<>")
		|| !ft_strcmp(next_token, "|") || !ft_strcmp(next_token, "||")
		|| !ft_strcmp(next_token, "&&") || !ft_strcmp(next_token, "(")
		|| !ft_strcmp(next_token, ")"))
	{
		parser_syntax_error(next_token);
		return (1);
	}
	return (0);
}
// static void ft_child_sigint(void)
// {
// 	//(void)signo;

// 	// rl_clear_history();
// 	// if (get_shell()->line)
// 	// 	free (get_shell()->line);
// 	// free_gc();
// 	signal(SIGINT, SIG_DFL);
// }

static void	ft_here_child(int pipe_fd[2], char *limiter)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	line = NULL;
	get_shell()->line = line;
	while (1)
	{
		ft_printf(COLOR_Y "> " COLOR_X);
		line = get_next_line(0);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		free(line);
	}
	if (line)
		free(line);
	close(pipe_fd[1]);
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
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		rl_clear_history();
		free_gc();
		exit(1);
	}
	pid = fork();
	if (pid == 0)
	{
		close(pipe_fd[0]);
		ft_here_child(pipe_fd, limiter);
		rl_clear_history();
		free_gc();
		exit(0);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipe_fd[0]), -1);
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
	if (new_redir->fd == -1)
		return (-1);
	new_redir->type = type;
	new_redir->value = ft_gcstrdup(CAT_CMD, next_token);
	new_redir->next = NULL;
	if (!cmd->redir)
		cmd->redir = new_redir;
	else
	{
		last = cmd->redir;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
	return (0);
}
