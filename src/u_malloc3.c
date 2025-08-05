/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_malloc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:55:49 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/04 23:56:27 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates a string and returns it in the garbage collector.
 */
char	*ft_gcstrdup(t_egccat cat, char *src)
{
	int		i;
	char	*copy;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	copy = (char *)ft_gcmalloc(cat, sizeof(char) * (i + 1));
	if (copy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

/**
 * @brief Duplicates a string up to a specified length and returns it
 * in the garbage collector.
 */
char	*ft_gcstrndup(t_egccat cat, char *src, ssize_t n)
{
	int		i;
	char	*copy;
	int		len;

	len = 0;
	while (src[len] != '\0' && len < n)
		len++;
	copy = (char *)ft_gcmalloc(cat, sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = src[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}

/**
 * @brief Joins two strings and returns a new string in the garbage collector.
 */
char	*ft_gcstrjoin(t_egccat cat, char *s1, char *s2)
{
	char	*word;
	char	*result;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = ft_gcmalloc(cat, len1 + len2 + 1);
	if (!result)
		return (NULL);
	word = result;
	if (s1)
		while (*s1)
			*word++ = *s1++;
	if (s2)
		while (*s2)
			*word++ = *s2++;
	*word = '\0';
	return (result);
}
