/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:50:25 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Extracts a substring from a given string.
 *
 * @param s The original string.
 * @param start The starting index of the substring.
 * @param len The maximum length of the substring.
 * @return A newly allocated substring, or NULL if allocation fails.
 *
 * This function returns a substring starting from index 'start'
 *  with a maximum length of 'len'.
 * If 'start' is greater than the length of 's', an empty string is returned.
 * The function ensures memory allocation safety and proper null termination.
 * If start is beyond the length of s, return an empty string
 * Adjust len to fit within the remaining length of s
 * Copy substring from s
 * Null-terminate the substring
 */

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s || start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

// int	main(void)
// {
// 	char str[] = "Hello, World!";
// 	char *substr;

// 	/* Extract "World" from "Hello, World!" */
// 	substr = ft_substr(str, 7, 5);
// 	if (substr)
// 	{
// 		printf("Original: \"%s\"\n", str);
// 		printf("Substring: \"%s\"\n", substr);
// 		free(substr);
// 	}
// 	else
// 		printf("Memory allocation failed.\n");

// 	return (0);
// }
