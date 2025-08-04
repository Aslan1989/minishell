/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putpointer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:20:25 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/17 14:05:32 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints a memory address (pointer).
 *
 * This function prints a pointer address in hexadecimal format.
 * It starts with "0x" and uses lowercase letters.
 * It also updates the count of printed characters.
 *ft_puthex(addr, 0, count) - Print the address in hex
 * @param ptr The pointer to print.
 * @param count A pointer to the number of printed characters.
 */
int	ft_putpointer(void *ptr, int *count)
{
	unsigned long	addr;

	addr = (unsigned long)ptr;
	if (write(1, "0x", 2) == -1)
		return (-1);
	*count += 2;
	if (addr == 0)
	{
		if (write(1, "0", 1) == -1)
			return (-1);
		(*count)++;
		return (0);
	}
	return (ft_puthex(addr, 0, count));
}
