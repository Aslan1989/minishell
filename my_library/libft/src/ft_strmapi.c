/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:32:46 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:39:26 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Applies a function to each character of a string.
 *
 * @param s The input string to modify.
 * @param f The function to apply to each character.
 * @return A newly allocated string with modified characters, or NULL on failure.
 *
 * This function iterates over the given string `s`, applying function `f`
 * to each character along with its index, and stores the result in a new string.
 * Apply function `f` to each character
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_str;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	new_str = (char *)malloc(ft_strlen(s) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

// char	to_uppercase(unsigned int i, char c)
// {
// 	(void)i; // Suppress unused parameter warning
// 	if (c >= 'a' && c <= 'z')
// 		return (c - 32);
// 	return (c);
// }

// int	main(void)
// {
// 	char *original = "hello, 42!";
// 	char *modified = ft_strmapi(original, to_uppercase);

// 	if (modified)
// 	{
// 		printf("Original: %s\n", original);
// 		printf("Modified: %s\n", modified);
// 		free(modified); // Free allocated memory
// 	}
// 	return (0);
// }
