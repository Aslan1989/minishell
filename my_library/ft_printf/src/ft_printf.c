/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:26:08 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/17 14:35:43 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
/**
 * @brief Handles different format specifiers for printing.
 *
 * This function checks the format character and calls the appropriate
 * function to handle the specific format (e.g., characters, strings, integers).
 * It also updates the count of printed characters.
 *
 * @param format The format specifier (e.g., 'c', 's', 'd', etc.).
 * @param args A list of arguments corresponding to the format specifiers.
 * @param count A pointer to the number of printed characters.
 */
static int	ft_handle_format(const char format, va_list args, int *count)
{
	if (format == 'c')
		return (ft_putchar(va_arg(args, int), count));
	else if (format == 's')
		return (ft_putstr(va_arg(args, char *), count));
	else if (format == 'd' || format == 'i')
		return (ft_putnbr(va_arg(args, int), count));
	else if (format == 'u')
		return (ft_putunsigned(va_arg(args, unsigned int), count));
	else if (format == 'x' || format == 'X')
		return (ft_puthex(va_arg(args, unsigned int), format == 'X', count));
	else if (format == 'p')
		return (ft_putpointer(va_arg(args, void *), count));
	else if (format == '%')
	{
		if (write(1, "%", 1) == -1)
			return (-1);
		(*count)++;
	}
	return (0);
}

/**
 * @brief Custom implementation of printf function.
 *
 * This function simulates the behavior of the standard printf function.
 * It processes the input string, handles format specifiers,
 * and prints the result.
 * count - how much characters were written
 * @param format The format string that specifies how to print the arguments.
 * @return The total number of characters printed.
 */

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	i = 0;
	count = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			if (ft_handle_format(format[i], args, &count) == -1)
				return (va_end(args), -1);
		}
		else if (ft_putchar(format[i], &count) == -1)
			return (va_end(args), -1);
		i++;
	}
	va_end(args);
	return (count);
}
