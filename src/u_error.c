/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_error.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 14:00:33 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 11:48:23 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_error(const char *msg)
{
	int	len;

	len = ft_strlen(msg);
	if (len > 0)
	{
		write(2, msg, len);
		return (len);
	}
	else
	{
		write(2, "minishell: error: empty message\n", 33);
		return (33);
	}
}

void	print_arg_err(const char *arg, const char *msg)
{
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	print_errno_for(const char *what)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(what, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void	parser_syntax_error(const char *tok)
{
	get_shell()->syntax_error = 1;
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	if (tok && *tok)
		ft_putstr_fd((char *)tok, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}
