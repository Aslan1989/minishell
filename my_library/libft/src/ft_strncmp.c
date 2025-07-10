/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:31:22 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:42:54 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Compares up to 'size' characters of two strings.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @param size The maximum number of characters to compare.
 * @return An integer less than, equal to, or greater than zero
 *         if s1 is found to be less than, equal to, or greater than s2.
 *
 * This function compares the first 'size' characters of two strings,
 * stopping when a difference is found or when the null terminator is reached.
 * If size is 0, return 0 because we compared the full requested length
 */
int	ft_strncmp(const char *s1, const char *s2, size_t size)
{
	while (size > 0 && *s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
		size--;
	}
	if (size == 0)
	{
		return (0);
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

// int main (void)
// {
// 	char str1[] = "abcdef";
// 	char str2[] = "abcxyz";
// 	char str3[] = "abc";

// 	if (ft_strncmp(str1, str2, 4) == 0)
// 	{
// 		printf("equal\n");
// 	}
// 	else
// 	{
// 		printf("not equal\n");
// 	}

// 	if (ft_strncmp(str1, str3, 3) == 0)
// 	{
// 		printf("equal\n");
// 	}
// 	else
// 	{
// 		printf("not equal\n");
// 	}
// 	int result1 = ft_strncmp(str1, str2, 4);
// 	printf("%d\n", result1);
// 	int result2 = ft_strncmp(str1, str3, 3);
// 	printf("%d\n", result2);
// 	int result3 = ft_strncmp(str2, str3, 4);
// 	printf("%d\n", result3);

// 	return (0);
// }
