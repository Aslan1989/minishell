/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_malloc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:55:49 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/04 23:56:27 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds a new node to the garbage collector list.
 */
t_garbage	*ft_gc_addback(t_garbage **lst, void *ptr)
{
	t_garbage	*last;
	t_garbage	*new_node;

	if (!lst || !ptr)
		return (NULL);
	new_node = malloc(sizeof(t_garbage));
	if (!new_node)
		return (NULL);
	new_node->ptr = ptr;
	new_node->next = NULL;
	if (*lst == NULL)
	{
		*lst = new_node;
		return (new_node);
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new_node;
	return (new_node);
}

/**
 * @brief Frees all nodes in the garbage collector list for a given category.
 */
void	free_gc_cat(t_egccat cat)
{
	t_garbage	*current;
	t_garbage	*next;
	t_garbage	**gc_list;

	gc_list = get_gc(cat);
	if (!gc_list || !*gc_list)
		return ;
	current = *gc_list;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	*gc_list = NULL;
}

/**
 * @brief Frees all nodes in the garbage collector list.
 */
void	free_gc(void)
{
	int	i;

	i = 0;
	while (i < CAT_MAX)
	{
		free_gc_cat(i);
		i++;
	}
}

/**
 * @brief Frees a pointer and removes it from the garbage collector.
 */
void	ft_gcfree(t_egccat cat, void *ptr)
{
	t_garbage	**gc_list;
	t_garbage	*cur;
	t_garbage	*prev;

	gc_list = get_gc(cat);
	if (!gc_list || !*gc_list || !ptr)
		return ;
	cur = *gc_list;
	prev = NULL;
	while (cur)
	{
		if (cur->ptr == ptr)
		{
			if (prev)
				prev->next = cur->next;
			else
				*gc_list = cur->next;
			free(cur->ptr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}
