/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:14:43 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:47:28 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Checks if a character is in the given set.
 *
 * @param c The character to check.
 * @param set The set of characters to compare against.
 * @return 1 if 'c' is in 'set', 0 otherwise.
 */
static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}
/**
 * @brief Trims characters from the beginning and end of a string based on 'set'.
 *
 * @param s1 The original string to trim.
 * @param set The set of characters to remove from the start and end of 's1'.
 * @return A newly allocated trimmed string, or NULL if allocation fails.
 *
 * This function removes leading and trailing characters that appear in 'set'.
 * It does not modify the original string and returns
 *  a new dynamically allocated string.
 * Find the first non-set character
 * Find the last non-set character
 * Allocate memory for the trimmed string
 * Copy trimmed portion of the string
 */

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in_set(s1[end - 1], set))
		end--;
	trimmed = (char *)malloc(end - start + 1);
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1 + start, end - start);
	trimmed[end - start] = '\0';
	return (trimmed);
}

// int	main(void)
// {
// 	char str[] = "   ***Hello World!***   ";
// 	char set[] = " *";
// 	char *trimmed_str;

// 	trimmed_str = ft_strtrim(str, set);
// 	if (trimmed_str)
// 	{
// 		printf("Original: \"%s\"\n", str);
// 		printf("Trimmed:  \"%s\"\n", trimmed_str);
// 		free(trimmed_str);
// 	}
// 	else
// 		printf("Memory allocation failed.\n");

// 	return (0);
// }
