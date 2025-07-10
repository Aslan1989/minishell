/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:37:49 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:08:50 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Writes a character to the specified file descriptor.
 *
 * @param c The character to write.
 * @param fd The file descriptor to which the character is written.
 *
 * This function uses the `write` system call to output the character 'c'
 * to the given file descriptor. Useful for writing to standard output (fd = 1),
 * standard error (fd = 2), or files.
 * Write the character 'c' to the given file descriptor 'fd'
 */
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// #include <fcntl.h>

// int main()
// {
//     ft_putchar_fd('A', 1);
//     return 0;
// int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return 1; // Ошибка при открытии файла

//     ft_putchar_fd('X', fd); // write 'X' in file "output.txt"
//     close(fd);
// }
