/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:01:30 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:00:21 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Copies 'n' bytes from source to destination.
 *
 * @param dest Pointer to the destination buffer.
 * @param src Pointer to the source buffer.
 * @param n Number of bytes to copy.
 * @return A pointer to the destination buffer.
 *
 * Copies 'n' bytes from 'src' to 'dest'. If both 'dest' and 'src' are NULL,
 * the function returns NULL. It does not handle overlapping memory regions.
 * Cast the input pointers to unsigned char pointers for byte-wise copying
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	if (!dest && !src)
		return (NULL);
	while (n--)
		*d++ = *s++;
	return (dest);
}

// int main(void)
// {
// 	char src[] = "High Voltage!";
//     char dest[20];
//     ft_memcpy(dest, src, ft_strlen(src) + 1);
//     printf("After memcpy: %s\n", dest);
// 	printf("\n");
// }
