/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:48:05 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 13:41:05 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_if_opened(int fd)
{
	if (fd >= 0)
		close(fd);
}

static void	close_with_error(char *msg)
{
	perror(msg);
	exit(1);
}

static void	check_redirection(t_redir *current, t_cmd *cmd)
{
	if (current->type == REDIR_IN)
	{
		close_if_opened(cmd->fd_in);
		cmd->fd_in = open(current->value, O_RDONLY);
		if (cmd->fd_in < 0)
			close_with_error(current->value);
	}
	else if (current->type == REDIR_OUT)
	{
		close_if_opened(cmd->fd_out);
		cmd->fd_out = open(current->value,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->fd_out < 0)
			close_with_error(current->value);
	}
	else if (current->type == REDIR_APPEND)
	{
		close_if_opened(cmd->fd_out);
		cmd->fd_out = open(current->value,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (cmd->fd_out < 0)
			close_with_error(current->value);
	}
}

void	setup_redirections(t_cmd *cmd)
{
	t_redir	*current;

	current = cmd->redir;
	while (current)
	{
		check_redirection(current, cmd);
		if (current->type == REDIR_HEREDOC)
		{
			close_if_opened(cmd->fd_in);
			cmd->fd_in = current->fd;
		}
		current = current->next;
	}
	if (cmd->fd_in >= 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) < 0)
			close_with_error("dup2 failed for STDIN");
		close(cmd->fd_in);
	}
	if (cmd->fd_out >= 0)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
			close_with_error("dup2 failed for STDOUT");
		close(cmd->fd_out);
	}
}
