/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:45:18 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:12:57 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Writes an integer to the specified file descriptor.
 *
 * @param n The integer to write.
 * @param fd The file descriptor to which the number is written.
 *
 * This function handles negative numbers, including the minimum value
 * (-2147483648), and prints the number recursively using `write()`.
 * Handle the special case for INT_MIN (-2147483648)
 * Recursively print digits for numbers greater than 9
 * Convert the last digit to a character and write it
 */
void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}
// #include <fcntl.h>

// int main()
// {
// 	ft_putnbr_fd(12345, 1);
// 	ft_putnbr_fd(-9876, 1);

//     int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return (1);

//     ft_putnbr_fd(42, fd);

//     close(fd);
//     return (0);
// }
