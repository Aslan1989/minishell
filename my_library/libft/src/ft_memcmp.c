/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:48:39 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 16:00:04 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Compares two memory blocks byte by byte.
 *
 * @param s1 The first memory block.
 * @param s2 The second memory block.
 * @param n The number of bytes to compare.
 * @return An integer greater than, equal to, or less than 0 if 's1' is
 *         found to be greater than, equal to, or less than 's2'.
 *
 * Iterates through 'n' bytes of both memory blocks.
 * If a difference is found, returns the difference between the bytes.
 * If no difference is found within 'n' bytes, returns 0.
 * Cast the input pointers to unsigned char pointers for byte-wise comparison
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}

// int main(void)
// {
//     char str1[] = "Hello, World!";
//     char str2[] = "Hello, world!";
//     char str3[] = "Hello, World!";

//     /* Compare str1 and str2 (difference in case sensitivity) */
//     int result1 = ft_memcmp(str1, str2, 13);
//     printf("Comparison of str1 and str2: %d\n", result1);
// Output: nonzero (case-sensitive difference)

//     /* Compare str1 and str3 (identical strings) */
//     int result2 = ft_memcmp(str1, str3, 13);
//     printf("Comparison of str1 and str3: %d\n", result2);
// Output: 0 (identical strings)

//     /* Compare only first 6 bytes (they are the same) */
//     int result3 = ft_memcmp(str1, str2, 6);
//     printf("Comparison of first 6 bytes: %d\n", result3);
// Output: 0 (first 6 bytes are identical)

//     return (0);
// }
