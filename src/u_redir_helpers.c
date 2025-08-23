/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_redir_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:26 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 19:37:19 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_std(int from_fd, int dst_fd, int *save_slot)
{
	if (from_fd < 0)
		return (0);
	*save_slot = dup(dst_fd);
	if (*save_slot < 0 || dup2(from_fd, dst_fd) < 0)
	{
		ft_putstr_fd("minishell: dup2: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		if (*save_slot >= 0)
			close(*save_slot);
		close(from_fd);
		return (1);
	}
	close(from_fd);
	return (0);
}

/**
 * @brief Duplicate prepared FDs onto STDIN/STDOUT in the parent process.
 *
 * Saves the current STDIN/STDOUT in @p save_in/@p save_out so they can be
 * restored later (see parent_stdio_restore). On any dup/dup2 error it prints
 * a diagnostic, closes temp FDs, and returns 1.
 *
 * @param fd_in    Prepared input FD (or -1 for none).
 * @param fd_out   Prepared output FD (or -1 for none).
 * @param save_in  Out: previous STDIN duplicate (or -1 when unused).
 * @param save_out Out: previous STDOUT duplicate (or -1 when unused).
 * @return int     0 on success, 1 on error.
 */
int	apply_fds_parent(int fd_in, int fd_out, int *save_in, int *save_out)
{
	if (redirect_std(fd_in, STDIN_FILENO, save_in))
	{
		if (fd_out >= 0)
			close(fd_out);
		return (1);
	}
	if (redirect_std(fd_out, STDOUT_FILENO, save_out))
		return (1);
	return (0);
}

int	open_inout(const char *path, int *fd_in)
{
	if (*fd_in >= 0)
		close(*fd_in);
	*fd_in = open(path, O_RDWR | O_CREAT, 0644);
	if (*fd_in < 0)
		return (print_errno_for(path), 1);
	return (0);
}
