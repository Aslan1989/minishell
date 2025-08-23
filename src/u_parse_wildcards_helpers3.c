/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:13:01 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 20:13:42 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pmatch_after_star(const char *pat, const char *str)
{
	if (!*pat)
		return (1);
	while (*str)
	{
		if (pmatch(pat, str))
			return (1);
		str++;
	}
	return (0);
}

void	sort_strings(char **arr)
{
	int	n;
	int	i;
	int	j;

	n = arr_len(arr);
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - 1 - i)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				swap_str(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}

int	append_result(char ***res, int *count, const char *s)
{
	char	**r;

	r = (char **)ft_gcrealloc(CAT_ARGS, *res,
			sizeof(char *) * (*count + 2));
	if (!r)
		return (1);
	r[*count] = ft_gcstrdup(CAT_ARGS, s);
	if (!r[*count])
		return (1);
	(*count)++;
	r[*count] = NULL;
	*res = r;
	return (0);
}

int	want_show_hidden(const char *pattern)
{
	if (pattern && pattern[0] == '.')
		return (1);
	return (0);
}

int	scan_entries(DIR *dir, const char *pat, int show_dot,
				char ***res, int *count, int *matched)
{
	struct dirent	*ent;
	const char		*name;

	ent = readdir(dir);
	while (ent)
	{
		name = ent->d_name;
		if ((show_dot || name[0] != '.') && pmatch(pat, name))
		{
			if (append_result(res, count, name))
				return (1);
			*matched = 1;
		}
		ent = readdir(dir);
	}
	return (0);
}
