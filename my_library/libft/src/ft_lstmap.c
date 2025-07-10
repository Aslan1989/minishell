/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <bonscott.isaev@mail.ru>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:20:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/20 10:41:10 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Creates a new list by applying a function to each node's content.
 *
 * @param lst The first node of the original list.
 * @param f A function to apply to each node's content.
 * @param del A function used to delete content in case of allocation failure.
 * @return A new list with transformed content, or NULL if allocation fails.
 *
 * Iterates through the list, applying 'f' to each node's content.
 * Creates a new node for each transformed content and adds it to the new list.
 * If memory allocation fails, clears the newly created list and returns NULL.
 * new_content = f(lst->content);
 * Apply function 'f' to transform the current node's content
 * new_node = ft_lstnew(new_content);
 * Create a new node with the transformed content
 * del(new_content);
 * Free the transformed content in case of allocation failure
 * ft_lstclear(&new_list, del);
 * Clear the new list and free allocated nodes
 * lst = lst->next;
 * Move to the next node in the original list
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*new_content;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}

// void	*double_number(void *content)
// {
// 	int *new_num = malloc(sizeof(int));
// 	if (!new_num)
// 		return (NULL);
// 	*new_num = *(int *)content * 2;
// 	return (new_num);
// }

// void	del_number(void *content)
// {
// 	free(content);
// }

// int	main()
// {
// 	int a = 1, b = 2, c = 3;
// 	t_list *lst = ft_lstnew(&a);
// 	ft_lstadd_back(&lst, ft_lstnew(&b));
// 	ft_lstadd_back(&lst, ft_lstnew(&c));

// 	t_list *new_lst = ft_lstmap(lst, double_number, del_number);

// 	// Вывод нового списка
// 	t_list *tmp = new_lst;
// 	while (tmp)
// 	{
// 		printf("%d\n", *(int *)tmp->content);
// 		tmp = tmp->next;
// 	}

// 	// Очищаем память
// 	ft_lstclear(&new_lst, del_number);

// 	return (0);
// }
