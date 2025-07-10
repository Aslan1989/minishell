/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:49:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:33:34 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Concatenates two strings, ensuring null termination and size limit.
 *
 * @param dst The destination buffer.
 * @param src The source string to append.
 * @param dstsize The total size of the destination buffer.
 * @return The total length of the string it tried to create
 *         (initial dst length + src length).
 *
 * This function appends src to dst, ensuring that the result is null-terminated
 * if dstsize > 0. It does not append more than dstsize - strlen(dst) - 1 bytes.
 * Calculate the lengths of dst and src
 * If dstsize is less than or equal to dst_len,
 * return what would have been the full length
 * Append src to dst while ensuring we do not exceed dstsize - 1
 * Return the total length of the string it tried to create
 */
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	i = 0;
	while (src[i] && (dst_len + i) < (dstsize - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}

// int	main(void)
// {
// 	char buffer[20] = "Hello, ";
// 	const char *add = "World!";
// 	size_t total_length;

// 	total_length = ft_strlcat(buffer, add, sizeof(buffer));
// 	printf("Resulting String: %s\n", buffer);
// 	printf("Total Length: %zu\n", total_length);

// 	return (0);
// }
