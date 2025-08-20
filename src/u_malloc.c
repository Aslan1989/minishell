/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_malloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:55:49 by psmolin           #+#    #+#             */
/*   Updated: 2025/08/04 23:56:02 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates memory in the garbage collector.
 */
void	*ft_gcmalloc(t_egccat cat, ssize_t size)
{
	void		*ptr;
	t_garbage	*new_node;

	if (cat < 0 || cat >= CAT_MAX || size <= 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new_node = ft_gc_addback(get_gc(cat), ptr);
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->size = size;
	return (ptr);
}

/**
 * @brief Reallocates memory in the garbage collector.
 */
void	*ft_gcrealloc(t_egccat cat, void *ptr, ssize_t size)
{
	void	*new_ptr;
	size_t	old_size;
	size_t	copy_size;

	if (cat < 0 || cat >= CAT_MAX || size <= 0)
		return (NULL);
	if (!ptr)
		return (ft_gcmalloc(cat, size));
	new_ptr = ft_gcmalloc(cat, size);
	if (!new_ptr)
		return (NULL);
	old_size = ft_gc_getsize(cat, ptr);
	if (old_size < (size_t)size)
		copy_size = old_size;
	else
		copy_size = (size_t)size;
	if (copy_size > 0)
		ft_memcpy(new_ptr, ptr, copy_size);
	ft_gcfree(cat, ptr);
	return (new_ptr);
}

/**
 * @brief Returns a link to the garbage collector list for a given category.
 */
t_garbage	**get_gc(t_egccat cat)
{
	static t_garbage	*gc[CAT_MAX];

	if (cat < 0 || cat >= CAT_MAX)
		return (NULL);
	return (&gc[cat]);
}

/**
 * @brief Gets the size of a memory block in the garbage collector.
 */
size_t	ft_gc_getsize(t_egccat cat, void *ptr)
{
	t_garbage	*current;

	if (cat < 0 || cat >= CAT_MAX || !ptr)
		return (0);
	current = *get_gc(cat);
	while (current)
	{
		if (current->ptr == ptr)
			return (current->size);
		current = current->next;
	}
	return (0);
}
