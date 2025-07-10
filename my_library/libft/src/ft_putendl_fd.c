/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:43:51 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:09:58 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Writes a string followed by a newline to the specified file descriptor.
 *
 * @param s The string to write.
 * @param fd The file descriptor to which the string is written.
 *
 * This function writes the string 's' to the given file descriptor 'fd'
 * using the `write` system call, followed by a newline character ('\n').
 * If 's' is NULL, the function does nothing.
 * Check for NULL input to avoid segmentation fault
 * Write the string 's' to the given file descriptor 'fd'
 */
void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

// #include <fcntl.h>

// int main()
// {
// 	ft_putendl_fd("Hello, World!", 1);
// 	ft_putendl_fd("Error: Something went wrong!", 2);
//     int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return 1; // Ошибка при открытии файла

//     ft_putendl_fd("Hello, File!", fd);
//     close(fd);
//     return 0;
// }
