/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:25:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:35:02 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Copies a string with size limit, ensuring null termination.
 *
 * @param dst The destination buffer.
 * @param src The source string.
 * @param size The total size of the destination buffer.
 * @return The total length of the source string (strlen(src)).
 *
 * This function copies up to (size - 1) characters from src to dst, ensuring
 * that dst is null-terminated. If size is 0, only the length of src is returned.
 * Copy src to dst, but limit it to size - 1 characters
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	src_len;

	src_len = 0;
	while (src[src_len])
		src_len++;
	if (size == 0)
		return (src_len);
	i = 0;
	while (src[i] && i + 1 < size)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

// int	main(void)
// {
// 	char buffer[10];
// 	const char *source = "Hello, World!";
// 	size_t copied_length;

// 	copied_length = ft_strlcpy(buffer, source, sizeof(buffer));
// 	printf("Copied String: %s\n", buffer);
// 	printf("Source Length: %zu\n", copied_length);

// 	return (0);
// }
