/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse5_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/05 20:58:40 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*open_quote_word(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg;
	while (**arg && **arg != '"' && **arg != '\'')
		(*arg)++;
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	ret = ft_expand_env(ret);
	return (ret);
}

static char	*open_quote_single(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '\'')
		(*arg)++;
	if (*arg == start)
		return (NULL);
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	if (**arg == '\'')
		(*arg)++;
	return (ret);
}

static char	*open_quote_double(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '"')
		(*arg)++;
	if (*arg == start)
		return (NULL);
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	ret = ft_expand_env(ret);
	if (**arg == '"')
		(*arg)++;
	return (ret);
}

void	ft_open_quotes(t_arg *arg)
{
	char	*new_arg;
	char	*sub_arg;
	char	*line;

	if (!arg)
		return ;
	arg->wildcard = 1;
	new_arg = NULL;
	line = arg->arg;
	while (*line)
	{
		if (*line == '"' || *line == '\'')
		{
			if (*line == '"')
				sub_arg = open_quote_double(&line);
			else
				sub_arg = open_quote_single(&line);
			if (ft_strpbrk(sub_arg, "*?[]"))
				arg->wildcard = 0;
		}
		else
			sub_arg = open_quote_word(&line);
		new_arg = ft_gcstrjoin(CAT_ARGS, new_arg, sub_arg);
	}
	arg->arg = new_arg;
}
