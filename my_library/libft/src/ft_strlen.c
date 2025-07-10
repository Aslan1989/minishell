/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:45:41 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:35:46 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Computes the length of a string.
 *
 * @param s The string whose length is to be calculated.
 * @return The number of characters in the string
 * (excluding the null terminator).
 *
 * This function iterates through the string until it reaches the null
 * terminator ('\0'), counting the number of characters.
 */
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

// int	main(void)
// {
// 	const char *str = "Hello, 42!";
// 	size_t length;

// 	length = ft_strlen(str);
// 	printf("Length of \"%s\": %zu\n", str, length);

// 	return (0);
// }
