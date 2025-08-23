/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:59:09 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 15:01:29 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Consume a plain word segment (no quotes) and expand env vars in it.
 */
char	*open_quote_word(char **arg)
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

/**
 * @brief Consume a single-quoted segment (no expansions inside).
 */
char	*open_quote_single(char **arg)
{
	char	*ret;
	char	*start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '\'')
		(*arg)++;
	if (*arg == start)
	{
		ret = ft_gcstrdup(CAT_ARGS, "");
		if (**arg == '\'')
			(*arg)++;
		return (ret);
	}
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	if (**arg == '\'')
		(*arg)++;
	return (ret);
}
