/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:37:58 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 16:00:35 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_invalid_identifier(char *key)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static void	process_key_value_with_equal(t_shell *shell, char *arg, char *equal)
{
	char	*key;
	char	*value;

	key = arg;
	value = equal + 1;
	*equal = '\0';
	if (!is_valid_identifier(key))
	{
		print_invalid_identifier(key);
		*equal = '=';
		return ;
	}
	if (!replace_env_var(shell->envp, key, value))
		add_env_var(shell, key, value);
	*equal = '=';
}

static void	process_key_value_without_equal(t_shell *shell, char *arg)
{
	char	*key;

	key = arg;
	if (!is_valid_identifier(key))
	{
		print_invalid_identifier(key);
		return ;
	}
	if (!replace_env_var(shell->envp, key, ""))
		add_env_var(shell, key, "");
}

static void	process_key_value(t_shell *shell, char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
		process_key_value_with_equal(shell, arg, equal);
	else
		process_key_value_without_equal(shell, arg);
}


int	built_export(t_shell *shell, char **args)
{
	int		i;
	int		ret;
	char	*eq;
	char	saved;
	if (!args[1])
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (is_option_arg(args[i]))
			ret = 2;
		else
		{
			eq = ft_strchr(args[i], '=');
			if (eq)
			{
				saved = *eq;
				*eq = '\0';
				if (!is_valid_identifier(args[i]) && ret == 0)
					ret = 1;
				*eq = saved;
			}
			else if (!is_valid_identifier(args[i]) && ret == 0)
				ret = 1;
		}
		i++;
	}
	i = 1;
	while (args[i])
	{
		process_key_value(shell, args[i]);
		i++;
	}
	return (ret);
}
