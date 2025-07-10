/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:48:55 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 14:52:21 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Scans memory for the first occurrence of a character.
 *
 * @param s The memory block to search.
 * @param c The character to search for (interpreted as an unsigned char).
 * @param n The number of bytes to search.
 * @return A pointer to the first occurrence of 'c' in 's',
 *         or NULL if 'c' is not found within the first 'n' bytes.
 *
 * Iterates through the memory block, comparing each byte with 'c'.
 * If a match is found, returns a pointer to the matching byte.
 * Otherwise, returns NULL.
 *
 * ptr = (unsigned char *)s: Cast the input pointer to an unsigned char pointer
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)&ptr[i]);
		i++;
	}
	return (NULL);
}

// int main(void)
// {
// 	char str_memchr[] = "Hello, world!";
// 	char *ptr_memchr;

// 	// looking for 'o' in first 10 bytes
// 	ptr_memchr = ft_memchr(str_memchr, 'o', 10);
// 	if (ptr_memchr)
// 		printf("First 'o' found at: %s\n", ptr_memchr);
// 	else
// 		printf("'o' not found\n");
// 	// looking for 'z' (doesnt exist)
// 	ptr_memchr  = ft_memchr(str_memchr, 'z', 10);
// 	if (ptr_memchr )
// 		printf("Found 'z': %s\n", ptr_memchr);
// 	else
// 		printf("'z' not found\n");
// 	// looking for '\0' (end of string)
// 	ptr_memchr  = ft_memchr(str_memchr, '\0', 15);
// 	if (ptr_memchr )
// 		printf("Null terminator found at: %p\n", (void *)ptr_memchr);
// }
