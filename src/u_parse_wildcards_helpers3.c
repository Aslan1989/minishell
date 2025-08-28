/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:13:01 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/28 10:50:29 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Try matching the rest of pattern after a '*' against str.
 *
 * Implements greedy '*' logic: if pattern ended → success;
 * otherwise shift str one char at a time and try pmatch() recursively.
 *
 * '*' at the end matches everything
 * Try every suffix of str
 * If the rest of pattern matches this suffix
 * Advance one character and try again
 * No suffix matched
 * @param pat Pattern starting after one or more '*'.
 * @param str Candidate string to test.
 * @return int 1 if matches, 0 otherwise.
 */
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

/**
 * @brief Append a string to a growing (GC-managed) argv array.
 *
 * Reallocates (*res) to hold one more entry + NULL terminator and copies s.
 *
 * @param res   [in/out] Pointer to the argv pointer.
 * @param count [in/out] Current number of items in *res.
 * @param s     String to append.
 * @return int 0 on success, 1 on allocation failure.
 */
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

/**
 * @brief Decide whether hidden files (starting with '.') should be matched.
 *
 * Like bash (with default options): only match dotfiles if pattern itself
 * starts with '.'.
 *
 * @param pattern The wildcard pattern.
 * @return int 1 if dotfiles should be considered, 0 otherwise.
 */
int	want_show_hidden(const char *pattern)
{
	if (pattern && pattern[0] == '.')
		return (1);
	return (0);
}

/**
 * @brief Scan directory entries and collect names that match a pattern.
 *
 * Skips dotfiles unless ctx->show_dot is set.
 * For every match calls append_result().
 *
 * @param dir Open directory handle.
 * @param ctx Wildcard context (pattern, output array, counters, flags).
 * @return int 0 on success, 1 on append/allocation failure.
 */
int	scan_entries(DIR *dir, t_wc_ctx *ctx)
{
	struct dirent	*ent;
	const char		*name;

	ent = readdir(dir);
	while (ent)
	{
		name = ent->d_name;
		if ((ctx->show_dot || name[0] != '.') && pmatch(ctx->pat, name))
		{
			if (append_result(ctx->res, ctx->count, name))
				return (1);
			ctx->matched = 1;
		}
		ent = readdir(dir);
	}
	return (0);
}

/**
 * @brief Expand a single wildcard pattern in the current directory.
 *
 * Opens ".", filters entries using pmatch(), respects dotfile rule,
 * appends matches to (*res), sorts them if any, and returns number of matches.
 *
 * Scan current directory
 * Can't open → no matches
 * Only include dotfiles if pattern starts with '.'
 * Append all matches
 * Return number already collected on error
 * Keep expansion results sorted (like bash)
 * @param pat   Pattern to expand (e.g., "*.c").
 * @param res   [in/out] Pointer to argv-like result array.
 * @param count [in/out] Number of items stored in *res.
 * @return int Number of matches (0 if none or error opening dir).
 */
int	expand_one_pattern(const char *pat, char ***res, int *count)
{
	DIR			*dir;
	t_wc_ctx	c;
	int			start;

	dir = opendir(".");
	if (!dir)
		return (0);
	start = *count;
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
		sort_strings_range(*res, start, *count - 1);
	return (c.matched);
}
