/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:12:03 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:54:53 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Adds a new element at the beginning of a linked list.
 *
 * @param lst A pointer to the pointer of the first element of the list.
 * @param new The new element to be added at the front of the list.
 *
 * If 'lst' or 'new' is NULL, the function does nothing.
 * Point the new node's 'next' to the current first node
 * Update the list's head to the new node
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

// int main()
// {
//     t_list *head = NULL;

//     ft_lstadd_front(&head, ft_lstnew("3rd"));
//     ft_lstadd_front(&head, ft_lstnew("2nd"));
//     ft_lstadd_front(&head, ft_lstnew("1st"));

//     printf("First: %s\n", (char *)head->content);
//     printf("Second: %s\n", (char *)head->next->content);
//     printf("Third: %s\n", (char *)head->next->next->content);

//     free(head->next->next);
//     free(head->next);
//     free(head);
//     return (0);
// }
