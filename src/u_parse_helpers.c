/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 14:59:09 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/23 18:56:23 by aisaev           ###   ########.fr       */
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

void	skip_ws(const char **line)
{
	while (**line && ft_isspace(**line))
		(*line)++;
}

int	is_redir_word(t_arg *arg)
{
	if (!arg || arg->quoted)
		return (0);
	if (!ft_strcmp(arg->arg, "<"))
		return (1);
	if (!ft_strcmp(arg->arg, ">"))
		return (1);
	if (!ft_strcmp(arg->arg, ">>"))
		return (1);
	if (!ft_strcmp(arg->arg, "<<"))
		return (1);
	return (0);
}

int	ensure_capacity(t_cmd *node, int *cap, int next_idx)
{
	t_arg	**grown;
	int		new_cap;

	if (next_idx + 1 < *cap)
		return (0);
	if (*cap < 4)
		new_cap = 4;
	else
		new_cap = *cap * 2;
	grown = (t_arg **)ft_gcrealloc(CAT_ARGS, node->args,
			sizeof(t_arg *) * new_cap);
	if (!grown)
		return (1);
	node->args = grown;
	*cap = new_cap;
	return (0);
}

int	append_arg(t_cmd *node, t_arg *arg, int *i, int *cap)
{
	if (ensure_capacity(node, cap, *i))
		return (1);
	node->args[*i] = arg;
	*i += 1;
	node->args[*i] = NULL;
	return (0);
}
