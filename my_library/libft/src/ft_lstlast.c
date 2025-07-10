/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:14:35 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:56:59 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Returns the last node of a linked list.
 *
 * @param lst The first node of the list.
 * @return The last node of the list, or NULL if the list is empty.
 *
 * Iterates through the list until the last node is found.
 * If 'lst' is NULL, the function returns NULL.
 * Traverse the list until the last node is reached
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

// int main()
// {
//     t_list *head = ft_lstnew("First");
//     ft_lstadd_front(&head, ft_lstnew("Second"));
//     ft_lstadd_front(&head, ft_lstnew("Third"));

//     t_list *last = ft_lstlast(head);

//     printf("Last node content: %s\n", (char *)last->content);

//     return (0);
// }
