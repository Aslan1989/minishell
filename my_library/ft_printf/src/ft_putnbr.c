/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:53:46 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/15 12:59:56 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints an integer number.
 *
 * This function prints an integer number. If the number is negative,
 * it prints the minus sign. It also updates the count of printed characters.
 *
 * c = (n % 10) + '0' - Get the last digit
	write(1, &c, 1) - Print the digit
	(*count)++ - Increase the printed characters count
 * @param n The integer number to print.
 * @param count A pointer to the number of printed characters.
 */
int	ft_putnbr(int n, int *count)
{
	char	c;

	if (n < 0)
	{
		if (write(1, "-", 1) == -1)
			return (-1);
		(*count)++;
		if (n == -2147483648)
		{
			write(1, "2147483648", 10);
			*count += 10;
			return (0);
		}
		n = -n;
	}
	if (n >= 10)
	{
		if (ft_putnbr(n / 10, count) == -1)
			return (-1);
	}
	c = (n % 10) + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count)++;
	return (0);
}
