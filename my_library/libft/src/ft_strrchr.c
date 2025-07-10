/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:12:51 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:45:44 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Locates the last occurrence of 'c' in the string 's'.
 *
 * @param s The string to search in.
 * @param c The character to find (converted to unsigned char).
 * @return A pointer to the last occurrence of 'c' in 's',
 *         or NULL if 'c' is not found.
 *
 * This function scans the string 's' from start to end, updating the
 * 'last' pointer each time 'c' is found. If 'c' is '\0', it returns
 * a pointer to the null terminator at the end of 's'.
 * Update 'last' whenever 'c' is found
 * If 'c' is '\0', return pointer to null terminator
 */
char	*ft_strrchr(char *s, int c)
{
	char	*last;

	last = NULL;
	while (*s)
	{
		if (*s == (char)c)
			last = (char *)s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)last);
}
