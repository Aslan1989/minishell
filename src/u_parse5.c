/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/04 23:39:46 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_getenv(char *var)
{
	t_shell	*shell;
	char	*env_value;
	int		i;
	int		len;

	shell = get_shell();
	if (!shell || !shell->envp)
		return (NULL);
	if (ft_strncmp(var, "?", 2) == 0)
		return (ft_gcstrdup(CAT_ARGS, ft_itoa(shell->last_exit_status)));
	i = 0;
	len = ft_strlen(var);
	env_value = NULL;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], var, len) == 0 && shell->envp[i][len] == '=')
		{
			env_value = shell->envp[i] + len + 1;
			return (ft_gcstrdup(CAT_ARGS, env_value));
		}
		i++;
	}
	return (ft_gcstrdup(CAT_ARGS, ""));
}

static char *ft_getwordenv(char **line)
{
	char *start;

	start = *line + 1;
	(*line)++;
	if (**line == '?')
	{
		(*line)++;
		return (ft_getenv("?"));
	}
	while (**line && ft_char_is_good_for_env(**line))
		(*line)++;
	return (ft_getenv(ft_gcstrndup(CAT_ARGS, start, *line - start)));
}

static char *ft_getword(char **line)
{
	char	*start;

	start = *line;
	while (**line && **line != '$')
		(*line)++;
	return (ft_gcstrndup(CAT_ARGS, start, *line - start));
}

static char	*ft_expand_env(char *str)
{
	char	*line;
	char	*sub_line;
	char	*ret;

	line = str;
	ret = NULL;
	while (*line)
	{
		if (*line == '$')
			sub_line = ft_getwordenv(&line);
		else
			sub_line = ft_getword(&line);
		ret = ft_gcstrjoin(CAT_ARGS, ret, sub_line);
	}
	return (ret);
}

static char *open_quote_word(char **arg)
{
	char *ret;
	char *start;

	start = *arg;
	while (**arg && **arg != '"' && **arg != '\'')
		(*arg)++;
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	ret = ft_expand_env(ret);
	// ft_printf("Extracted word: %s\n", ret); // Debugging line
	return (ret);
}

static char *open_quote_single(char **arg)
{
	char *ret;
	char *start;

	start = *arg + 1;
	(*arg)++;
	while (**arg && **arg != '\'')
		(*arg)++;
	if (*arg == start)
		return (NULL);
	ret = ft_gcstrndup(CAT_ARGS, start, *arg - start);
	if (**arg == '\'')
		(*arg)++;
	// ft_printf("Extracted quote: %s\n", ret); // Debugging line
	return (ret);
}

static char *open_quote_double(char **arg)
{
	char *ret;
	char *start;

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
	// ft_printf("Extracted double quote: %s\n", ret); // Debugging line
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
			if (ft_strchr(sub_arg, '*'))
				arg->wildcard = 0;
		}
		else
			sub_arg = open_quote_word(&line);
		new_arg = ft_gcstrjoin(CAT_ARGS, new_arg, sub_arg);
	}
	arg->arg = new_arg;
}
