/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:47:27 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:06:37 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Fills the first 'n' bytes of memory with a specified byte value.
 *
 * @param s Pointer to the memory block to fill.
 * @param c The value to be set (converted to an unsigned char).
 * @param n Number of bytes to set.
 * @return A pointer to the memory block 's'.
 *
 * This function sets each of the first 'n' bytes of the memory block
 * pointed to by 's' to the specified byte value 'c'.
 * Cast input pointer to unsigned char for byte-wise operations
 * Set 'n' bytes to the specified value
 */
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = (unsigned char)c;
	return (s);
}

// int main(void)
// {
// 	printf("check ft_memset\n");
// 	char	str[] = "Hells Bells";
// 	printf("Before memset: %s\n", str);
// 	ft_memset(str, 'A', 5);
// 	printf("After memset: %s\n", str);
// }
