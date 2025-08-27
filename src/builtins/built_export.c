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
 * @brief Handle KEY=VALUE case for `export`.
 *
 * Validates KEY; on success replaces or adds KEY with VALUE in env.
 *
 * @param shell Shell context (provides envp and helpers).
 * @param arg   Original argument string containing '=' (e.g., "FOO=bar").
 * @param equal Pointer to the '=' inside @p arg.
 * @note Temporarily writes '\0' at '=' to validate KEY, then restores '='.
 * @sideeffect May modify the environment (replace or add variable).
 */
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

/**
 * @brief Handle KEY (without '=') case for `export`.
 *
 * Validates KEY; if valid, ensures KEY exists in env with empty value.
 *
 * @param shell Shell context.
 * @param arg   Argument without '=' (e.g., "FOO").
 * @sideeffect May modify the environment (create KEY with empty string).
 */
static void	process_key_value_without_equal(t_shell *shell, char *arg)
{
	char	*key;
	(void)shell;
	key = arg;
	if (!is_valid_identifier(key))
	{
		print_invalid_identifier(key);
		return ;
	}
	if (!replace_env_var(shell->envp, key, ""))
		add_env_var(shell, key, "");
}

/**
 * @brief Dispatch processing of an export argument (with or without '=').
 *
 * @param shell Shell context.
 * @param arg   The raw export argument (e.g., "KEY" or "KEY=VALUE").
 * @sideeffect May modify the environment.
 */
static void	process_key_value(t_shell *shell, char *arg)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
		process_key_value_with_equal(shell, arg, equal);
	else
		process_key_value_without_equal(shell, arg);
}

/**
 * @brief Validate a single export argument.
 *
 * @param arg Argument string (KEY or KEY=VALUE).
 * @param ret Current return code (0,1,2).
 * @return int Updated return code.
 */
static int	check_export_arg(char *arg, int ret)
{
	char	*eq;
	char	saved;

	if (is_option_arg(arg))
		return (2);
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		saved = *eq;
		*eq = '\0';
		if (!is_valid_identifier(arg) && ret == 0)
			ret = 1;
		*eq = saved;
	}
	else if (!is_valid_identifier(arg) && ret == 0)
		ret = 1;
	return (ret);
}

/**
 * @brief `export` builtin: print or set environment variables.
 *
 * Behavior:
 *  - With no args: prints the environment sorted as `declare -x ...`.
 *  - With args: validates identifiers and sets variables (KEY or KEY=VALUE).
 *  - Option-like args (starting with '-') set return code to 2.
 *
 * Return codes (aggregated over all args):
 *  - 0: all identifiers valid (or only printing).
 *  - 1: at least one invalid identifier was found.
 *  - 2: at least one option-like argument was seen (unsupported option).
 *
 * @param shell Shell context (holds envp and helpers).
 * @param args  NULL-terminated argv; args[0] == "export".
 * @return int See return code semantics above.
 * @sideeffect May print to STDOUT/STDERR and modify the environment.
 */
int	built_export(t_shell *shell, char **args)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		print_sorted_env(shell->envp);
		return (0);
	}
	ret = 0;
	i = 1;
	while (args[i])
	{
		ret = check_export_arg(args[i], ret);
		process_key_value(shell, args[i]);
		i++;
	}
	return (ret);
}
