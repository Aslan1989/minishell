/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:10:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 14:41:23 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Creates a new node for a linked list.
 *
 * @param content The content to store in the new node.
 * @return A pointer to the new node, or NULL if memory allocation fails.
 *
 * Allocates memory for a new node, initializes its content, and sets its
 * 'next' pointer to NULL. Returns the newly created node.
 * new_node->content = content: Assign content to the new node
 * new_node->next = NULL: Initialize next pointer to NULL
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

// int main()
// {
//     int value = 42;
//     t_list *node = ft_lstnew(&value);

//     if (node)
//         printf("Content: %d\n", *(int *)(node->content));

//     free(node);
//     return (0);
// t_list *node1 = ft_lstnew("First");
// t_list *node2 = ft_lstnew("Second");

// node1->next = node2; // Соединяем узлы

// printf("Node 1: %s\n", (char *)node1->content);
// printf("Node 2: %s\n", (char *)node1->next->content);

// free(node1);
// free(node2);
// }
