/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:46:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/04 23:23:45 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check whether a token is eligible for wildcard expansion.
 *
 * Conditions:
 *  - t_arg exists and its .wildcard flag is set (e.g., not quoted).
 *  - The string contains at least one of '*', '?', '[' tokens.
 *
 * Quoted args typically set wildcard=0
 * @param a Argument structure with fields: .arg (string), .wildcard (flag).
 * @return int 1 if it has wildcard tokens, 0 otherwise.
 */
int	has_wc_token(t_arg *a)
{
	if (!a || !a->wildcard)
		return (0);
	if (ft_strpbrk(a->arg, "*?["))
		return (1);
	return (0);
}

/**
 * @brief Expand wildcard arguments into a flat argv-like array.
 *
 * For each t_arg:
 *  - If it contains wildcard tokens and no '/' inside the token, call
 *    expand_one_pattern(). If it matches nothing, append the literal.
 *  - Otherwise, append the literal as-is.
 *
 * Returns a newly allocated (GC) array of char* (NULL-terminated)
 * or NULL if no args produced (res stays NULL).
 *
 * Result argv
 * Number of items in res
 * Index in args
 * Number of matches for a pattern
 * If the token contains '/'
 * Iterate all parsed args
 * Globbing disabled across '/'
 * Only expand simple filename patterns
 * On append failure, return what we have
 * No globbing → append literal
 * May be NULL if no items were appended
 * @param args NULL-terminated array of t_arg* (each has .arg and .wildcard).
 * @return char** Expanded argument vector (may be NULL if empty).
 */
char	**ft_expand_wildcards(t_arg **args)
{
	char	**res;
	int		cnt;
	int		i;
	int		matched;
	int		has_slash;

	res = NULL;
	cnt = 0;
	i = 0;
	while (args && args[i])
	{
		has_slash = (ft_strchr(args[i]->arg, '/') != NULL);
		if (has_wc_token(args[i]) && !has_slash)
		{
			matched = expand_one_pattern(args[i]->arg, &res, &cnt);
			if (!matched && append_result(&res, &cnt, args[i]->arg))
				return (res);
		}
		else if (append_result(&res, &cnt, args[i]->arg))
			return (res);
		i++;
	}
	return (res);
}

/**
 * @brief Consume the next word as a redirection target and attach it to node.
 *
 * Reads the next argument from the input stream and converts current redirection
 * symbol into a redir node (IN, OUT, APPEND, INOUT, HEREDOC).
 * If the next token is missing or invalid, sets a syntax error.
 *
 * Read the filename / limiter after redir
 * Missing target → syntax error
 * Validate the next token (e.g., not another redir)
 * @param node  Command node being filled.
 * @param arg   Current token that is a redirection operator
 * ("<", ">", ">>", "<>", "<<").
 * @param line  Pointer to current position in the
 * input (advanced by extract_arg()).
 * @return int 0 on success; 1 on syntax error or redirection-setup error.
 */
static int	handle_redirection(t_cmd *node, t_arg *arg, const char **line)
{
	t_arg	*next;
	int		check;

	next = extract_arg(line);
	if (!next)
	{
		parser_syntax_error(NULL);
		return (1);
	}
	if (ft_redir_check_next(next->arg))
		return (1);
	if (!ft_strcmp(arg->arg, "<"))
		check = ft_redir_add(node, REDIR_IN, next->arg);
	else if (!ft_strcmp(arg->arg, ">"))
		check = ft_redir_add(node, REDIR_OUT, next->arg);
	else if (!ft_strcmp(arg->arg, ">>"))
		check = ft_redir_add(node, REDIR_APPEND, next->arg);
	else if (!ft_strcmp(arg->arg, "<>"))
		check = ft_redir_add(node, REDIR_INOUT, next->arg);
	else
		check = ft_redir_add(node, REDIR_HEREDOC, next->arg);
	return (check);
}

/**
 * @brief Fill t_cmd with parsed positional arguments and redirections.
 *
 * Iterates over the input line:
 *  - Skips whitespace.
 *  - Extracts the next t_arg via extract_arg().
 *  - If it's a redirection word, handle it; else append to
 * argv and grow capacity if needed.
 *
 * Start filling from index 0
 * Initial capacity hint
 * Scan until end of this command segment
 * Move past spaces/tabs
 * End reached
 * Parse the next argument (handles quotes, etc.)
 * Extraction failed → syntax error
 * Is this a redirection operator token?
 * Append to node->args; grow if needed
 * @param node  Command node to fill (node->args / node->commands).
 * @param line  Input command segment
 * (not modified; we pass a pointer copy inside helpers).
 * @param argc  Pre-counted number of arguments (capacity hint).
 * @param i     Output: number of args appended into node->args
 * (index of next slot).
 * @return int  0 on success, 1 on error
 * (syntax error, allocation failure, etc.).
 */
static int	ft_fillout_commands(t_cmd *node, const char *line, int argc, int *i)
{
	t_arg	*arg;
	int		cap;

	*i = 0;
	cap = argc;
	if (cap < 1)
		cap = 1;
	while (*line)
	{
		skip_ws(&line);
		if (!*line)
			break ;
		arg = extract_arg(&line);
		if (!arg)
			return (1);
		if (is_redir_word(arg))
		{
			if (handle_redirection(node, arg, &line))
				return (1);
		}
		else if (append_arg(node, arg, i, &cap))
			return (1);
	}
	return (0);
}

/**
 * @brief Parse a raw command line into t_cmd: build args and expand wildcards.
 *
 * Steps:
 *  1) Validate input and allocate node->args (capacity = count_args(line) + 1).
 *  2) Fill node->args by iterating tokens; also attach redirections to node.
 *  3) Null-terminate node->args.
 *  4) Expand wildcards into node->commands (argv-like char**).
 *
 * Clear outputs
 * Empty input → error for this node
 * Pre-count args to size the array
 * Parse args + redirections
 * NULL-terminate t_arg* array
 * Expand globs into flat argv
 * Keep argv only if non-NULL
 * On success, node->commands receives the expanded argv
 * (may be NULL if no args),
 * and node->args remains the structured array of parsed t_arg*.
 *
 * @param node Destination command node (fields .args and .commands are set).
 * @param line Raw command line segment for this node
 * (e.g., up to operator/pipe boundary).
 * @return int 0 on success, 1 on error.
 */
int	parse_input(t_cmd *node, const char *line)
{
	int		argc;
	int		i;
	char	**new_command;

	node->commands = NULL;
	node->args = NULL;
	if (!line || !*line)
		return (1);
	argc = count_args(line);
	node->args = ft_gcmalloc(CAT_ARGS, sizeof(t_arg *) * (argc + 1));
	if (!node->args)
		return (1);
	i = 0;
	if (ft_fillout_commands(node, line, argc, &i))
		return (1);
	node->args[i] = NULL;
	new_command = ft_expand_wildcards(node->args);
	if (new_command)
		node->commands = new_command;
	return (0);
}
