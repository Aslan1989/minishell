/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 13:21:04 by aisaev            #+#    #+#             */
/*   Updated: 2025/04/16 13:07:48 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(const char *format, ...);
int	ft_puthex( unsigned long n, int uppercase, int *count);
int	ft_putchar(char c, int *count);
int	ft_putunsigned(unsigned int n, int *count);
int	ft_putstr(const char *str, int *count);
int	ft_putpointer(void *ptr, int *count);
int	ft_putnbr(int n, int *count);

#endif
