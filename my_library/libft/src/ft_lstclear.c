/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:18:10 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:55:32 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Deletes and frees all elements of a linked list.
 *
 * @param lst A pointer to the pointer of the first element of the list.
 * @param del A function pointer used to delete the content of each node.
 *
 * Iterates through the list, deleting each node using ft_lstdelone().
 * Sets the list pointer to NULL after clearing all elements.
 * If 'lst' or 'del' is NULL, the function does nothing.
 * Store the next node before deleting the current one
 * Delete the current node using the provided 'del' function
 * Move to the next node
 * Ensure the list is fully cleared by setting the head to NULL 
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = temp;
	}
	*lst = NULL;
}

// void	del_content(void *content)
// {
// 	free(content);
// }

// int main()
// {
// 	t_list *lst = ft_lstnew(strdup("Hello"));
// 	ft_lstadd_back(&lst, ft_lstnew(strdup("World")));
// 	ft_lstadd_back(&lst, ft_lstnew(strdup("42")));

// 	printf("Before clear: %s\n", (char *)lst->content);
// 	ft_lstclear(&lst, del_content);
// 	printf("After clear: %p\n", (void *)lst);

// 	return (0);
// }
