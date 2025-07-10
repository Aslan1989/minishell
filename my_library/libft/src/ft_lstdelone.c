/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:17:18 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:56:16 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Deletes a single node from a linked list.
 *
 * @param lst The node to delete.
 * @param del A function pointer used to delete the content of the node.
 *
 * Calls the 'del' function to free the node's content.
 * Then frees the node itself.
 * If 'lst' or 'del' is NULL, the function does nothing.
 * Free the content of the node using the provided 'del' function
 */
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

// void	del_content(void *content)
// {
// 	free(content);
// }

// int main()
// {
// 	t_list *node = ft_lstnew(strdup("Hello!"));

// 	printf("Before: %s\n", (char *)node->content);
// 	ft_lstdelone(node, del_content);
// 	// Теперь node недоступен, доступ к нему вызовет ошибку
// 	return (0);
// }
