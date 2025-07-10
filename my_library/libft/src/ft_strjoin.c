/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:11:11 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:30:32 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Concatenates two strings into a newly allocated string.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 * @return A pointer to the newly allocated string containing s1 + s2,
 *         or NULL if memory allocation fails.
 *
 * This function allocates enough memory to store both strings, copies
 * them into the new memory, and returns the concatenated result.
 * Get the length of both input strings
 * Allocate memory for the concatenated string
 * Copy s1 into str ,Append s2 to str 
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = (char *)malloc(len1 + len2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	return (str);
}

// int	main(void)
// {
// 	char	*s1 = "Hello, ";
// 	char	*s2 = "Heilbronn!";
// 	char	*result;

// 	result = ft_strjoin(s1, s2);
// 	if (result)
// 	{
// 		printf("Joined String: %s\n", result);
// 		free(result);
// 	}
// 	else
// 		printf("Memory allocation failed.\n");

// 	return (0);
// }
