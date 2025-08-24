/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 13:48:05 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 14:24:21 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_if_opened(int fd)
{
	if (fd >= 0)
		close(fd);
}

/**
 * @brief Print an error with perror(), set last_exit_status, and exit(1).
 *
 * Adds "minishell: " prefix, then calls perror(msg) to show strerror(errno).
 * If the global shell exists, set its last_exit_status to 1.
 *
 * @param msg Context message, usually a path or API name (e.g., "dup2").
 */
static void	close_with_error(char *msg)
{
	t_shell	*shell;

	shell = get_shell();
	ft_putstr_fd("minishell: ", 2);
	perror(msg);
	if (shell)
		shell->last_exit_status = 1;
	exit(1);
}

/**
 * @brief Open/prepare a single redirection and store fds into cmd.
 *
 * Supports:
 *  - REDIR_IN     : open value for reading (O_RDONLY), set cmd->fd_in
 *  - REDIR_OUT    : open value for writing (truncate), set cmd->fd_out
 *  - REDIR_APPEND : open value for append (O_APPEND), set cmd->fd_out
 *
 * On open failure, prints an error and exits via close_with_error().
 *
 * @param current One redirection node (type + value).
 * @param cmd     Command being prepared (fd_in/fd_out are updated).
 */
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

/**
 * @brief Apply all redirections recorded in cmd, then dup2 them onto stdio.
 *
 * Walks the linked list cmd->redir:
 *  - For file-based redirs (<, >, >>) it opens the target path and stores
 *    the resulting fd into cmd->fd_in / cmd->fd_out.
 *  - For heredoc (REDIR_HEREDOC) it uses the already prepared pipe fd
 *    stored in current->fd as the new cmd->fd_in.
 *
 * Finally, if cmd->fd_in/out are valid (>= 0), duplicates them to
 * STDIN_FILENO / STDOUT_FILENO and closes the originals.
 * Any dup2() failure is fatal and exits the child.
 *
 * @param cmd Command holding the redirection list and fd slots.
 */
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
			close_with_error("dup2");
		close(cmd->fd_in);
	}
	if (cmd->fd_out >= 0)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) < 0)
			close_with_error("dup2");
		close(cmd->fd_out);
	}
}
