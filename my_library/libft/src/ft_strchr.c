/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:12:15 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:21:52 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Locates the first occurrence of a character in a string.
 *
 * @param s The string to search in.
 * @param c The character to find (converted to unsigned char).
 * @return A pointer to the first occurrence of 'c' in 's', or NULL if not found.
 *
 * This function searches for the first occurrence of the character 'c'
 * in the string 's'. If 'c' is found, a pointer to its position is returned.
 * If 'c' is the null terminator ('\0'), a pointer to the end of the string
 * is returned. If 'c' is not found, NULL is returned.
 * Iterate through the string to find 'c'
 * If 'c' is '\0', return a pointer to the null terminator
 */

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}

// int main(void)
// {
// 	const char	check_strchr[] = "Hello, world!";
// 	char		*ptr;
// 	ptr = ft_strchr(check_strchr, 'o');
// 	printf("First 'o' found at: %s\n", ptr);
// 	ptr = ft_strchr(check_strchr, 'z');
// 	if (ptr == NULL)
// 		printf("Looking for 'z': Not found\n");
// 	else
// 		printf("Looking for 'z': %s\n", ptr);
// 	ptr = ft_strchr(check_strchr, '\0');
// 	printf("Null terminator found at: %p\n", (void *)ptr);
// }
