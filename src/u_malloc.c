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

void	*ft_gcmalloc(t_egccat cat, ssize_t size)
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
	new_node->size = size;
	return (ptr);
}

size_t		ft_gc_getsize(t_egccat cat, void *ptr)
{
	t_garbage	*current;

	if (cat < 0 || cat > CAT_MAX || !ptr)
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

void	*ft_gcrealloc(t_egccat cat, void *ptr, ssize_t size)
{
	void	*new_ptr;
	ssize_t	old_size;
	ssize_t copy_size;

	if (cat < 0 || cat > CAT_MAX || size <= 0)
		return (NULL);
	if (!ptr)
		return (ft_gcmalloc(cat, size));
	new_ptr = ft_gcmalloc(cat, size);
	if (!new_ptr)
		return (NULL);
	old_size = ft_gc_getsize(cat, ptr);
	if (size > old_size)
		copy_size = old_size;
	else
		copy_size = size;
	ft_memcpy(new_ptr, ptr, copy_size);
	ft_gcfree(cat, ptr);
	return (new_ptr);
}

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

char	*ft_gcstrdup(t_egccat cat, char *src)
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

char	*ft_gcstrndup(t_egccat cat, char *src, ssize_t n)
{
	int		i;
	char	*copy;
	int		len;

	len = 0;
	while (src[len] != '\0' && len < n)
		len++;
	copy = (char *)ft_gcmalloc(cat, sizeof(char) * (len + 1));
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = src[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}
