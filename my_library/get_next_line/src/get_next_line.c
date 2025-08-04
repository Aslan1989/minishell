/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:54:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/23 13:52:22 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

/**
 * Join two strings and free the first string.
 *
 * @param s1 First string (will be freed).
 * @param s2 Second string to add.
 * @return New joined string, or NULL if malloc fails.
 */
static char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*res;
	size_t	i;
	size_t	j;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), NULL);
	i = 0;
	while (i < len1)
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
		res[i++] = s2[j++];
	res[i] = '\0';
	free(s1);
	return (res);
}

/**
 * Get one line from the stash (until '\n' or end).
 *
 * @param stash Buffer with the saved data.
 * @return One line from stash, or NULL if stash is empty.
 */
static char	*get_line(char *stash)
{
	int		i;
	int		j;
	char	*line;

	if (!stash || !stash[0])
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	if (i == 0)
		return (NULL);
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	j = 0;
	while (j < i)
	{
		line[j] = stash[j];
		j++;
	}
	line[i] = '\0';
	return (line);
}

/**
 * Remove the line from stash and keep the rest.
 *
 * @param stash Buffer with the saved data.
 * @return Updated stash without the first line, or NULL if empty.
 */
static char	*update_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = 0;
	j = 0;
	if (!stash)
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i++])
	{
		free(stash);
		return (NULL);
	}
	new_stash = malloc(ft_strlen(stash) -i + 1);
	if (!new_stash)
		return (free(stash), NULL);
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

/**
 * Read from a file and return the next line each time it's called.
 *
 * @param fd File descriptor.
 * @return Next line from the file, or NULL if end or error.
 */
char	*get_next_line(int fd)
{
	static char	*stash;
	char		buffer[BUFFER_SIZE + 1];
	ssize_t		bytes;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes = 1;
	while (!ft_strchr(stash, '\n') && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (free_stash_and_return_null(&stash));
		buffer[bytes] = '\0';
		stash = ft_strjoin(stash, buffer);
		if (!stash)
			return (NULL);
	}
	line = get_line(stash);
	stash = update_stash(stash);
	if (!line || line[0] == '\0')
		return (free_line_and_stash(line, &stash));
	return (line);
}
