/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_malloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:55:49 by psmolin           #+#    #+#             */
/*   Updated: 2025/07/16 00:15:10 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_garbage	*ft_gc_addback(t_garbage **lst, void *ptr)
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

t_garbage **get_gc(e_gccat cat)
{
	static t_garbage *gc[CAT_MAX];

	if (cat < 0 || cat > CAT_MAX)
		return (NULL);
	return (&gc[cat]);
}

void	free_gc_cat(e_gccat cat)
{
	t_garbage *current;
	t_garbage *next;
	t_garbage **gc_list;

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
	*gc_list = NULL; // Clear the garbage collector for this category
}

void	free_gc(void)
{
	int i;

	i = 0;
	while (i < CAT_MAX)
	{
		free_gc_cat(i);
		i++;
	}
}
char	*ft_gcstrdup(e_gccat cat, char *src)
{
	int		i;
	char	*copy;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	copy = (char *)ft_gcmalloc(cat, sizeof(char) * (i + 1));
	if (copy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	*ft_gcmalloc(e_gccat cat, ssize_t size)
{
	void		*ptr;
	t_garbage	*new_node;

	if (cat < 0 || cat > CAT_MAX || size <= 0)
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
	return (ptr);
}
