/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 22:22:06 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/28 12:53:42 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_buffer	buffer;
	t_tline			line;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (ft_initializeline(&line) == -1)
		return (NULL);
	if (buffer.length == 0)
		buffer.length = read(fd, buffer.text, BUFFER_SIZE);
	while (line.ended == 0 && buffer.length > 0)
	{
		ft_appendline(&line, &buffer);
		if (!line.text)
			return (NULL);
		if (buffer.length == 0 && line.ended == 0)
			ft_readbuffer(&buffer, fd);
	}
	if (line.text_length == 0 || buffer.length == -1)
		return (free(line.text), NULL);
	line.text = ft_gnlrealloc(line.text, line.text_length, line.text_length);
	return (line.text);
}
