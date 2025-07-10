/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:15:38 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 13:53:55 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Adds a new element to the end of a linked list.
 *
 * @param lst A pointer to the pointer of the first element of the list.
 * @param new The new element to be added to the list.
 *
 * If the list is empty, 'new' becomes the first element.
 * If 'lst' or 'new' is NULL, the function does nothing.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

// int main()
// {
//     t_list *head = ft_lstnew("First");
//     ft_lstadd_back(&head, ft_lstnew("Second"));
//     ft_lstadd_back(&head, ft_lstnew("Third"));

//     printf("Last node content: %s\n", (char *)ft_lstlast(head)->content);

//     return (0);
// }
