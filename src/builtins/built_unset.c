/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_unset.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:37:05 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:20:17 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *key)
{
	if (!key || !*key || !(ft_isalpha(*key) || *key == '_'))
		return (0);
	while (*key)
	{
		if (!(ft_isalnum(*key) || *key == '_'))
			return (0);
		key++;
	}
	return (1);
}

static int	match_key(const char *env_var, const char *key)
{
	size_t	len;

	len = ft_strlen(key);
	return (ft_strncmp(env_var, key, len) == 0 && env_var[len] == '=');
}

static int	should_remove(char *env_var, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i]) && match_key(env_var, args[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	print_invalid_identifiers(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_print_error("minishell: unset: `");
			ft_print_error((char *)args[i]);
			ft_print_error("': not a valid identifier\n");
		}
		i++;
	}
}

int	built_unset(t_shell *shell, char **args)
{
	int		i;
	int		j;
	char	**new_env;

	print_invalid_identifiers(args);
	i = 0;
	while (shell->envp[i])
		i++;
	new_env = ft_gcmalloc(CAT_ENV, sizeof(char *) * (i + 1));
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (!should_remove(shell->envp[i], args))
			new_env[j++] = shell->envp[i];
		i++;
	}
	new_env[j] = NULL;
	shell->envp = new_env;
	return (0);
}
