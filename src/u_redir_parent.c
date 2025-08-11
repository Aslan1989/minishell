/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir_parent.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:18:30 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/09 16:41:37 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Open an input file for reading and store FD in fd_in.
 *
 * Closes a previously opened fd_in (if any), then opens @p path in O_RDONLY.
 * On failure prints a bash-like diagnostic and returns 1.
 *
 * @param path  Filename to open for reading.
 * @param fd_in In/Out: current input FD to replace.
 * @return int  0 on success, 1 on error (fd_in unchanged on failure).
 */
static int	open_infile(const char *path, int *fd_in)
{
	if (*fd_in >= 0)
		close(*fd_in);
	*fd_in = open(path, O_RDONLY);
	if (*fd_in < 0)
		return (print_errno_for(path), 1);
	return (0);
}

/**
 * @brief Open an output file with truncation/append based on @p append.
 *
 * Closes a previously opened fd_out (if any), then opens @p path for write
 * with 0644 permissions. Uses O_TRUNC when append==0 and O_APPEND otherwise.
 * On failure prints a diagnostic and returns 1.
 *
 * @param path   Filename to open for writing.
 * @param append Non-zero for append (>>), zero for truncate (>).
 * @param fd_out In/Out: current output FD to replace.
 * @return int   0 on success, 1 on error (fd_out unchanged on failure).
 */
static int	open_outfile(const char *path, int append, int *fd_out)
{
	int	flags;

	if (*fd_out >= 0)
		close(*fd_out);
	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	*fd_out = open(path, flags, 0644);
	if (*fd_out < 0)
	{
		print_errno_for(path);
		return (1);
	}
	return (0);
}

static int	collect_parent_fds(t_cmd *cmd, int *fd_in, int *fd_out)
{
	t_redir	*cur;

	*fd_in = -1;
	*fd_out = -1;
	cur = cmd->redir;
	while (cur)
	{
		if (cur->type == REDIR_IN && open_infile(cur->value, fd_in))
			return (1);
		else if (cur->type == REDIR_OUT && open_outfile(cur->value, 0, fd_out))
			return (1);
		else if (cur->type == REDIR_APPEND
			&& open_outfile(cur->value, 1, fd_out))
			return (1);
		else if (cur->type == REDIR_HEREDOC)
		{
			if (*fd_in >= 0)
				close(*fd_in);
			*fd_in = cur->fd;
		}
		cur = cur->next;
	}
	return (0);
}

/**
 * @brief Apply command redirections in the parent (for builtins only).
 *
 * Iterates over @p cmd->redir, opens the necessary files (including using the
 * heredoc FD), and then calls apply_fds_parent() to swap STDIN/STDOUT.
 *
 * @note This function never exits the process: it returns 1 on error so the
 * caller can decide how to proceed.
 *
 * @param cmd      Command node with redirection list.
 * @param save_in  Out: previous STDIN duplicate for later restore.
 * @param save_out Out: previous STDOUT duplicate for later restore.
 * @return int     0 on success, 1 on error.
 */
int	parent_redirs_apply(t_cmd *cmd, int *save_in, int *save_out)
{
	int	fd_in;
	int	fd_out;

	*save_in = -1;
	*save_out = -1;
	if (collect_parent_fds(cmd, &fd_in, &fd_out))
		return (1);
	return (apply_fds_parent(fd_in, fd_out, save_in, save_out));
}

/**
 * @brief Restore original STDIN/STDOUT previously saved by parent_redirs_apply.
 *
 * Safe to call with -1 values; will just do nothing for that stream.
 *
 * @param save_in  Saved STDIN duplicate (or -1).
 * @param save_out Saved STDOUT duplicate (or -1).
 */
void	parent_stdio_restore(int *save_in, int *save_out)
{
	if (*save_in >= 0)
	{
		dup2(*save_in, STDIN_FILENO);
		close(*save_in);
		*save_in = -1;
	}
	if (*save_out >= 0)
	{
		dup2(*save_out, STDOUT_FILENO);
		close(*save_out);
		*save_out = -1;
	}
}
