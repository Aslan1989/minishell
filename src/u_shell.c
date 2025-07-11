/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_shell.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 12:47:25 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/11 13:08:09 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return (0);
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	printf(COLOR_G"freeing\n"COLOR_X);
	free(split);
	return (0);
}

t_shell *get_shell(void)
{
	static t_shell sh;

	return(&sh);
}
