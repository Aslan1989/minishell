/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:07:43 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:03:25 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Copies 'n' bytes from source to destination,
 *  					handling overlapping memory.
 *
 * @param dest Pointer to the destination buffer.
 * @param src Pointer to the source buffer.
 * @param n Number of bytes to copy.
 * @return A pointer to the destination buffer.
 *
 * Unlike `ft_memcpy`, `ft_memmove`
 * correctly handles cases where `src` and `dest`
 * overlap by copying in the correct order to prevent data corruption.
 * If 'dest' is before 'src', copy forward
 * If 'dest' is after 'src', copy backward to avoid data corruption
 */
void	*ft_memmove(void *dest, void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	d = dest;
	s = src;
	if (!dest && !src)
		return (NULL);
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
	{
		d += n;
		s += n;
		while (n--)
			*(--d) = *(--s);
	}
	return (dest);
}

// int main(void)
// {
// 	char check_memmove[] = "12345";
//     printf("Before memmove: %s\n", check_memmove);
//     ft_memmove(check_memmove + 1, check_memmove, 4);
//     printf("After memmove: %s\n", check_memmove);
// 	printf("\n");

// }
