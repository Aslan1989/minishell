/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:13:36 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 14:47:18 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Counts the number of nodes in a linked list.
 * @param lst The first node of the list.
 * @return The total number of nodes in the list.
 * Iterates through the linked list, incrementing a counter
 * for each node until reaching the end.
 * while (lst): Traverse the list and count the nodes
 */
int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
