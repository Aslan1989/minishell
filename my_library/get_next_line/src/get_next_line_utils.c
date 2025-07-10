/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:54:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/24 17:27:18 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
/**
 * Free both line and stash if something is wrong or no line.
 *
 * @param line Pointer to the current line.
 * @param stash Pointer to stash pointer (double pointer).
 * @return Always returns NULL.
 */
char	*free_line_and_stash(char *line, char **stash)
{
	free(line);
	free(*stash);
	*stash = NULL;
	return (NULL);
}

/**
 * Free stash and return NULL (for error handling).
 *
 * @param stash Pointer to stash pointer (double pointer).
 * @return Always returns NULL.
 */
char	*free_stash_and_return_null(char **stash)
{
	free(*stash);
	*stash = NULL;
	return (NULL);
}

/**
 * Find a character in a string.
 *
 * @param s The string to search.
 * @param c The character to find.
 * @return Pointer to the first match, or NULL if not found.
 */
char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/**
 * Get the length of a string.
 *
 * @param s The string to measure.
 * @return Number of characters (not counting '\0').
 */
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}
