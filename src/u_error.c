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

#include <minishell.h>

int	ft_print_error(char *msg)
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
