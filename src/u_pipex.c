/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_pipex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:26:34 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/11 15:49:17 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_wcount(const char *str, char div)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == div)
			count++;
		i++;
	}
	return (count);
}

char	**ft_split_pipes(char *str, char div)
{
	int	i;
	int wc;
	char	**result;

	if (!str || !*str)
		return (NULL);
	wc = ft_wcount(str, div);
	result = malloc(sizeof(char *) * (wc + 2));
	if (!result)
		return (NULL);
	i = 0;
	while (*str)
	{
		char *next_div = ft_strchr(str, div);
		if (next_div)
			result[i] = ft_substr(str, 0, next_div - str);
		else
			result[i] = ft_substr(str, 0, ft_strlen(str));
		if (!result[i])
		{
			ft_free_split(result);
			return (NULL);
		}
		if (next_div)
			str = next_div + 1;
		else
			str += ft_strlen(str);
		printf("result[%d] = %s\n", i, result[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}
