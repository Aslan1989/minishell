/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:06:56 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/17 10:52:41 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints a number in hexadecimal format.
 *
 * This function prints an unsigned long number as hexadecimal.
 * It uses uppercase or lowercase letters, and updates the count.
 *
 * ft_puthex(n / 16, uppercase, count);-Print the higher digits first
 * c = digits[n % 16] - Get the current digit
	write(1, &c, 1) - Print the digit
	(*count)++ - Increase the printed characters count
 * @param n The number to print.
 * @param uppercase If 1, use A-F. If 0, use a-f.
 * @param count A pointer to the number of printed characters.
 */
int	ft_puthex( unsigned long n, int uppercase, int *count)
{
	char	c;
	char	*digits;

	if (uppercase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	if (n >= 16)
	{
		if (ft_puthex(n / 16, uppercase, count) == -1)
			return (-1);
	}
	c = digits[n % 16];
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count)++;
	return (0);
}
