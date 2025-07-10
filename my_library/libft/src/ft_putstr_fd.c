/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:41:35 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:14:07 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Writes a string to the specified file descriptor.
 *
 * @param s The string to write.
 * @param fd The file descriptor to which the string is written.
 *
 * This function writes the string 's' to the given file descriptor 'fd'
 * using the `write` system call. If 's' is NULL, the function does nothing.
 * Write the string 's' to the given file descriptor 'fd'
 * Check for NULL input to prevent segmentation faults
 */
void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

// #include <fcntl.h>

// int main()
// {
// 	ft_putstr_fd("Hello, World!\n", 1);
// 	ft_putstr_fd("Error: Something went wrong!\n", 2);
//     int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd == -1)
//         return 1;

//     ft_putstr_fd("Hello, File!\n", fd);
//     close(fd);
//     return 0;
// }
