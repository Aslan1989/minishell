/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:02:53 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/24 13:10:33 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pmatch_qmark(const char **ppat, const char **pstr)
{
	if (!**pstr)
		return (0);
	(*ppat)++;
	(*pstr)++;
	return (1);
}

int	pmatch_char(const char **ppat, const char **pstr)
{
	if (**ppat != **pstr)
		return (0);
	(*ppat)++;
	(*pstr)++;
	return (1);
}

int	cc_range_trip(const char *p, char c, int *ok)
{
	char	a;
	char	b;

	if (p[1] == '-' && p[2] && p[2] != ']')
	{
		a = p[0];
		b = p[2];
		if ((a <= b && c >= a && c <= b) || (a > b && c >= b && c <= a))
			*ok = 1;
		return (3);
	}
	if (c == *p)
		*ok = 1;
	return (1);
}

int	match_char_class(const char **pp, char c)
{
	const char	*p;
	int			neg;
	int			ok;

	p = *pp;
	neg = 0;
	ok = 0;
	if (*p == '!' || *p == '^')
	{
		neg = 1;
		p++;
	}
	if (*p == ']' || *p == '\0')
		return (0);
	while (*p && *p != ']')
		p += cc_range_trip(p, c, &ok);
	if (*p == ']')
		p++;
	*pp = p;
	if (neg && ok)
		return (0);
	if (neg && !ok)
		return (1);
	return (ok);
}

int	expand_one_pattern(const char *pat, char ***res, int *count)
{
	DIR			*dir;
	t_wc_ctx	c;

	dir = opendir(".");
	if (!dir)
		return (0);
	c.pat = pat;
	c.show_dot = want_show_hidden(pat);
	c.res = res;
	c.count = count;
	c.matched = 0;
	if (scan_entries(dir, &c))
	{
		closedir(dir);
		return (c.matched);
	}
	closedir(dir);
	if (c.matched)
		sort_strings(*res);
	return (c.matched);
}
