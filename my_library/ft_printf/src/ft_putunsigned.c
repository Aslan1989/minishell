/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunsigned.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:03:26 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/15 11:19:20 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints an unsigned integer.
 *
 * This function prints an unsigned integer number by recursively
 * printing its digits. It also updates the count of printed characters.
 *
 * @param n The unsigned integer to print.
 * @param count A pointer to the number of printed characters.
 */
int	ft_putunsigned(unsigned int n, int *count)
{
	char	c;

	if (n >= 10)
	{
		if (ft_putunsigned(n / 10, count) == -1)
			return (-1);
	}
	c = (n % 10) + '0';
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count)++;
	return (0);
}
