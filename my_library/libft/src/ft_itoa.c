/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:28:52 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 13:23:55 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
   get_length: Calculates the number of digits in an integer,
   including space for the minus sign if the number is negative.
*/

static int	get_length(int n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

/*
   ft_itoa: Converts an integer to a string.
   Allocates memory and returns the string representation of the number.
*/
char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	nb;

	len = get_length(n);
	nb = n;
	if (nb < 0)
		nb = -nb;
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}

// #include <stdio.h>

// int main(void)
// {
// 	char *s1_itoa = ft_itoa(12345);
//     printf("12345 -> %s\n", s1_itoa);
//     free(s1_itoa);
//     char *s2_itoa = ft_itoa(-54321);
//     printf("-54321 -> %s\n", s2_itoa);
//     free(s2_itoa);
//     char *s3_itoa = ft_itoa(0);
//     printf("0 -> %s\n", s3_itoa);
//     free(s3_itoa);
//     char *s4_itoa = ft_itoa(-2147483648);
//     printf("-2147483648 -> %s\n", s4_itoa);
//     free(s4_itoa);
// }
