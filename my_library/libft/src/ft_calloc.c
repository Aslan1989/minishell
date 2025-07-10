/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:13:32 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 13:38:53 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
/*
    ft_calloc: Allocates memory for an array of 'nmemb' elements,
    each of 'size' bytes, and initializes all bytes to zero.
    - Computes the total memory size required.
    - Handles edge cases where 'nmemb' or 'size' is zero.
    - Prevents integer overflow in multiplication.
    - Allocates memory and initializes it using ft_bzero.
    - Returns a pointer to the allocated memory or NULL on failure.
*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;

	total_size = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (size && nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}
