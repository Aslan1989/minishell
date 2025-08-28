/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:02:53 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/28 10:50:05 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Match a single '?' against one character.
 *
 * @param ppat [in/out] Pattern pointer (moves past '?').
 * @param pstr [in/out] String pointer (consumes 1 char if present).
 * @return int 1 if there was a char to consume, 0 otherwise.
 */
int	pmatch_qmark(const char **ppat, const char **pstr)
{
	if (!**pstr)
		return (0);
	(*ppat)++;
	(*pstr)++;
	return (1);
}

/**
 * @brief Match a literal character in pattern against string.
 *
 * @param ppat [in/out] Pattern pointer.
 * @param pstr [in/out] String pointer.
 * @return int 1 if equal, 0 otherwise.
 */
int	pmatch_char(const char **ppat, const char **pstr)
{
	if (**ppat != **pstr)
		return (0);
	(*ppat)++;
	(*pstr)++;
	return (1);
}

/**
 * @brief Consume one char/range piece inside a bracket class and test c.
 *
 * Examples:
 *   - "a-c" consumes 3 pattern chars and sets *ok if c in [a..c]
 * (order-insensitive).
 *   - Single char sets *ok if equal and consumes 1 char.
 *
 * Match inside range
 * Consumed "x-y"
 * Single char match
 * Consumed single
 * @param p  Pattern pointer currently at a class element (not incl. ']').
 * @param c  Candidate character from the string.
 * @param ok [in/out] Set to 1 if any clause matched so far.
 * @return int Number of pattern chars consumed from p (1 or 3).
 */
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

/**
 * @brief Parse a complete bracket class and test character c against it.
 *
 * Supports leading '!' or '^' for negation. Advances *pp to the char after ']'.
 *
 * Negated class?
 * Did any element match?
 * Negation marker
 * Empty/invalid class
 * Iterate class elements
 * Skip closing bracket
 * Publish new position
 * Negated but matched → fail
 * Negated and no match → success
 * Non-negated → return match flag
 * @param pp [in/out] Pattern pointer positioned right after '[' on entry.
 * @param c  Candidate character to test.
 * @return int 1 if class matches (respecting negation), 0 otherwise.
 */
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

void	sort_strings_range(char **arr, int start, int end)
{
	int	i;
	int	j;

	if (!arr || start < 0 || end <= start)
		return ;
	i = start;
	while (i < end)
	{
		j = start;
		while (j < end - (i - start))
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				swap_str(&arr[j], &arr[j + 1]);
			j++;
		}
		i++;
	}
}
