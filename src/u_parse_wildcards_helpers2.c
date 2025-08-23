/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:07:39 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 20:10:24 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pmatch_bracket(const char **ppat, const char **pstr)
{
	const char	*tmp;

	(*ppat)++;
	if (!**pstr)
		return (0);
	tmp = *ppat;
	if (!match_char_class(&tmp, **pstr))
		return (0);
	*ppat = tmp;
	(*pstr)++;
	return (1);
}

int	pmatch_star(const char **ppat, const char *str)
{
	while (**ppat == '*')
		(*ppat)++;
	return (pmatch_after_star(*ppat, str));
}

void	swap_str(char **a, char **b)
{
	char	*t;

	t = *a;
	*a = *b;
	*b = t;
}

int	arr_len(char **arr)
{
	int	n;

	n = 0;
	while (arr && arr[n])
		n++;
	return (n);
}

int	pmatch(const char *pat, const char *str)
{
	while (*pat)
	{
		if (*pat == '*')
			return (pmatch_star(&pat, str));
		if (*pat == '?')
		{
			if (!pmatch_qmark(&pat, &str))
				return (0);
			continue ;
		}
		if (*pat == '[')
		{
			if (!pmatch_bracket(&pat, &str))
				return (0);
			continue ;
		}
		if (!pmatch_char(&pat, &str))
			return (0);
	}
	return (*str == '\0');
}
