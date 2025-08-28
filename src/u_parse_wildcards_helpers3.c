/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_wildcards_helpers3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 20:13:01 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/28 11:03:44 by aisaev           ###   ########.fr       */
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
 * @brief Expand a wildcard pattern in the current directory.
 *
 * Scans entries in ".", matches them against the given pattern, and appends
 * the matching names into the result array. If no matches are found, nothing
 * is appended. When matches are found, they are sorted alphabetically.
 *
 * @param pat   The wildcard pattern to expand (supports *, ?, [..] etc).
 * @param res   Address of a NULL-terminated array of strings (char ***).
 *              Matches will be appended here; memory is grown with GC helpers.
 * @param count Pointer to the current number of elements in @p res.
 *              Updated with the number of new matches added.
 *
 * @return int  Non-zero if any matches were found, 0 otherwise.
 *
 * @note The caller owns @p res, which is managed with your GC allocator.
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
