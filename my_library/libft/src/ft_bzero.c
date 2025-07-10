/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 21:51:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:53:23 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
    ft_bzero: Sets the first 'n' bytes of the memory area
    pointed to by 's' to zero.
    - Uses an unsigned char pointer for byte-wise manipulation.
    - Iterates through 'n' bytes, setting each to zero.
	Set each byte to zero until 'n' bytes are processed
*/
void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = s;
	while (n--)
		*ptr++ = 0;
}

// int main(void)
// {
// 	printf("check ft_bzero\n");
// 	char ch_bzero[] = "Hells Bells";
// 	printf("Befor bzero: %s\n", ch_bzero);
// 	ft_bzero(ch_bzero, 5);
//     printf("After bzero(string):%s\n", ch_bzero + 5);
//     printf("After bzero(bites): %s", ch_bzero);
// 	size_t i = 0;
// 	while (i < sizeof(ch_bzero))
// 	{
// 		if (ch_bzero[i] == '\0')
// 			printf("[0]");
// 		else
// 			printf("[%c]", ch_bzero[i]);
// 		i++;
// 	}
// }
