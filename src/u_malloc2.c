/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_malloc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:55:49 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/24 14:20:33 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_garbage	**get_gc(t_egccat cat)
{
	static t_garbage	*gc[CAT_MAX];

	if (cat < 0 || cat > CAT_MAX)
		return (NULL);
	return (&gc[cat]);
}

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
