/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/16 13:07:53 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Prints one character on the screen.
 *
 * This function writes one character to the screen
 * and adds 1 to the count.
 *
 * (*count)++ - Increase the printed characters count
 *
 * @param c The character to print.
 * @param count A pointer to the number of printed characters.
 */
int	ft_putchar(char c, int *count)
{
	if (write(1, &c, 1) == -1)
		return (-1);
	(*count)++;
	return (0);
}
