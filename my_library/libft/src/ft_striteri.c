/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:34:55 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:26:35 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Applies a function to each character of a string, passing its index.
 *
 * @param s The string to iterate over.
 * @param f The function to apply, which takes the
 * 			index and a pointer to the character.
 *
 * This function iterates through the string 's', applying the function 'f'
 * to each character while passing its index.
 *  If 's' or 'f' is NULL, the function does nothing.
 * Iterate through the string and apply the function
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

// void	to_uppercase(unsigned int i, char *c)
// {
// 	if (i % 2 == 0 && *c >= 'a' && *c <= 'z')
// 		*c -= 32;
// }

// int	main(void)
// {
// 	char	str[] = "hello, world!";

// 	printf("Before: %s\n", str);
// 	ft_striteri(str, to_uppercase);
// 	printf("After:  %s\n", str);

// 	return (0);
// }
