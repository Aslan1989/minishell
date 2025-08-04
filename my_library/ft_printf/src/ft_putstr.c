/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:34:04 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/15 11:30:46 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints a string.
 *
 * This function prints a string character by character.
 * If the string is NULL, it prints "(null)" instead.
 * It also updates the count of printed characters.
 *
 * @param str The string to print.
 * @param count A pointer to the number of printed characters.
 */
int	ft_putstr(const char *str, int *count)
{
	if (!str)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		*count += 6;
		return (0);
	}
	while (*str)
	{
		if (write(1, str, 1) == -1)
			return (-1);
		(*count)++;
		str++;
	}
	return (0);
}
