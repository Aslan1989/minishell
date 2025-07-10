/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 04:19:11 by aisaev            #+#    #+#             */
/*   Updated: 2025/03/14 15:56:43 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/**
 * @brief Applies a function to each element of a linked list.
 *
 * @param lst The first node of the list.
 * @param f A function pointer to apply to each node's content.
 *
 * Iterates through the list and applies the function 'f'
 * to the content of each node.
 * If 'lst' or 'f' is NULL, the function does nothing.
 * Apply the function 'f' to the current node's content
 * Move to the next node
 */
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

// void	to_uppercase(void *content)
// {
// 	char *str = (char *)content;
// 	while (*str)
// 	{
// 		*str = toupper(*str);
// 		str++;
// 	}
// }

// int	main()
// {
// 	t_list *lst = ft_lstnew(strdup("hello"));
// 	ft_lstadd_back(&lst, ft_lstnew(strdup("world")));

// 	ft_lstiter(lst, to_uppercase);

// 	t_list *tmp = lst;
// 	while (tmp)
// 	{
// 		printf("%s\n", (char *)tmp->content);
// 		tmp = tmp->next;
// 	}

// 	ft_lstclear(&lst, free); // Освобождаем память
// 	return (0);
// }
