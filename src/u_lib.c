/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_lib.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 18:51:01 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/04 23:55:35 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strpbrk(const char *s, const char *accept)
{
	int	i;
	int	j;

	if (!s || !accept)
		return (NULL);
	i = 0;
	while (s[i])
	{
		j = 0;
		while (accept[j])
		{
			if (s[i] == accept[j])
				return ((char *)&s[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}

size_t	ft_strspn(const char *s, const char *accept)
{
	size_t	count;
	size_t	i;
	size_t	j;

	count = 0;
	i = 0;
	while (s[i])
	{
		j = 0;
		while (accept[j])
		{
			if (s[i] == accept[j])
			{
				count++;
				break ;
			}
			j++;
		}
		if (!accept[j])
			break ;
		i++;
	}
	return (count);
}

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
	free(split);
	return (0);
}

void	ft_ignore_ac_av(int argc, char **argv)
{
	(void)argc;
	(void)argv;
}

int	ft_char_is_good_for_env(char c)
{
	if (c == '_')
		return (1);
	if (ft_isalnum(c))
		return (1);
	if (ft_isalpha(c))
		return (1);
	return (0);
}
