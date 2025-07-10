/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:16:47 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:24:10 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Duplicates a string by allocating memory and copying its contents.
 *
 * @param src The source string to duplicate.
 * @return A pointer to the newly allocated duplicate
 * 					string, or NULL if allocation fails.
 *
 * This function allocates enough memory for a copy of 'src',
 * copies the contents of 'src' into the new memory, and returns a pointer to it.
 * Allocate memory for the copy, including the null terminator
 */
char	*ft_strdup(char *src)
{
	int		i;
	char	*copy;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	copy = (char *)malloc(sizeof(char) * (i + 1));
	if (copy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

// int	main(void)
// {
// 	char	*original = "Hello, World!";
// 	char	*duplicate;

// 	duplicate = ft_strdup(original);
// 	if (duplicate)
// 	{
// 		printf("Original: %s\n", original);
// 		printf("Duplicate: %s\n", duplicate);
// 		free(duplicate);
// 	}
// 	else
// 		printf("Memory allocation failed.\n");

// 	return (0);
// }
