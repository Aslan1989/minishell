/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:48:26 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:44:25 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Locates the first occurrence of 'needle' in 'haystack',
 *        searching at most 'len' characters.
 *
 * @param haystack The string to search in.
 * @param needle The substring to search for.
 * @param len The maximum number of characters to search.
 * @return A pointer to the first occurrence of 'needle' in 'haystack',
 *         or NULL if 'needle' is not found.
 *
 * This function searches for 'needle' within the first 'len' characters
 * of 'haystack'. If 'needle' is an empty string, 'haystack' is returned.
 * Compare characters while they match and stay within 'len'
 * If we reached the end of needle, return the match
 */
char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && (i + j) < len)
		{
			if (!needle[j + 1])
				return ((char *)&haystack[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char *big = "Hello 42 Network";
// 	char *small = "42";

// 	/* Search for '42' within the first 12 characters */
// 	char *result = ft_strnstr(big, small, 12);
// 	if (result)
// 		printf("Found: %s\n", result);
// 	else
// 		printf("Not found\n");

// 	/* Search for 'Network' within the first 10 characters (not found) */
// 	result = ft_strnstr(big, "Network", 10);
// 	if (result)
// 		printf("Found: %s\n", result);
// 	else
// 		printf("Not found\n");

// 	return (0);
// }
