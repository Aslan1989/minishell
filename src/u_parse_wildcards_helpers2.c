/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:07:39 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/24 18:46:03 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Match bracket class '[...]' at *ppat with current char of *pstr.
 *
 * Supports ranges (e.g., a-z, z-a), single chars, and negation ('!' or '^').
 *
 * Skip '['No char to match
 * Try class against current char
 * Advance past closing ']'
 * Consume matched char
 * @param ppat [in/out] Pattern pointer, advanced to char after ']'.
 * @param pstr [in/out] String pointer, advanced by one on success.
 * @return int 1 on match, 0 on mismatch.
 */
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

/**
 * @brief Consume consecutive '*' and apply star matching logic.
 *
 * Collapses runs of '*' then defers to pmatch_after_star().
 *
 * Treat multiple '*' as one
 * @param ppat [in/out] Pattern pointer (will be advanced).
 * @param str  String pointer (not consumed here).
 * @return int 1 if rest matches, 0 otherwise.
 */
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

/**
 * @brief Core wildcard pattern matcher for '*', '?', and bracket classes.
 *
 * Sequentially walks the pattern:
 *  - '*' → call pmatch_star().
 *  - '?' → matches any single char (pmatch_qmark()).
 *  - '[' → bracket char class (pmatch_bracket()).
 *  - else → exact char match (pmatch_char()).
 *
 * Consume one char
 * One char against class
 * Exact char must match
 * Pattern ended → string must also end
 * A match is valid only if the pattern ends AND the string is fully consumed.
 *
 * @param pat Pattern.
 * @param str String to test.
 * @return int 1 if matches, 0 otherwise.
 */
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
