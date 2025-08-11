/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   u_parse6_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:23 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/09 13:48:14 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Lookup environment variable by name and duplicate value in CAT_ARGS.
 * Supports the special case `?` → last exit status.
 */
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
		if (ft_strncmp(shell->envp[i], var, len) == 0
			&& shell->envp[i][len] == '=')
		{
			env_value = shell->envp[i] + len + 1;
			return (ft_gcstrdup(CAT_ARGS, env_value));
		}
		i++;
	}
	return (ft_gcstrdup(CAT_ARGS, ""));
}

/**
 * @brief Read a $VAR (or $?) token and return its expanded value.
 */
static char	*ft_getwordenv(char **line)
{
	char	*start;

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

/**
 * @brief Read a literal span until '$' or end.
 */
static char	*ft_getword(char **line)
{
	char	*start;

	start = *line;
	while (**line && **line != '$')
		(*line)++;
	return (ft_gcstrndup(CAT_ARGS, start, *line - start));
}

/**
 * @brief Expand environment variables in a string.
 * Concatenates literal and $VAR/$? segments.
 */
char	*ft_expand_env(char *str)
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

/**
 * @brief Acceptable characters for environment names.
 */
int	ft_char_is_good_for_env(char c)
{
	if (c == '_')
		return (1);
	if (ft_isalnum(c))
		return (1);
	if (ft_isalpha(c))
		return (1);
	return (0);
}
