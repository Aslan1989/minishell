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

/**
 * @brief Implements the `export` built-in command.
 *
 * Adds or updates environment variables in the shell's environment.
 * Example: export VAR=value
 * This function supports syntax checking and safely modifies the internal
 * envp array.
 *
 * @param shell Pointer to the shell structure (contains envp).
 * @param args The array of strings, where args[0] = "export",
 * args[1] = "VAR=value", etc.
 * @return int 0 on success, 1 on invalid format or allocation failure.
 */
static void	print_invalid_identifier(const char *key)
{
	ft_print_error("minishell: export: invalid identifier: ");
	ft_print_error(key);
	ft_print_error("\n");
}

static void	process_key_value_with_equal(t_shell *shell, char *arg, char *equal)
{
	const char	*key;
	const char	*value;

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
	const char	*key;

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
	int	i;

	if (!args[1])
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		process_key_value(shell, args[i]);
		i++;
	}
	return (0);
}
