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

/**
 * @brief Validate an identifier (A-Z a-z _ followed by A-Z a-z 0-9 _).
 *
 * @param key Candidate key.
 * @return int 1 if valid, 0 otherwise.
 */
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

/**
 * @brief Print diagnostics for invalid identifiers in args.
 */
static void	print_invalid_identifiers(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		i++;
	}
}

/**
 * @brief Rebuild envp without any variables listed in args.
 *
 * Strategy:
 *  - Count current env entries.
 *  - Allocate a new environment array with the same capacity.
 *  - Copy all entries except those whose keys appear in args.
 *  - Free the old envp if it was GC-owned, then swap in the new one.
 *
 * @param shell Shell state holding envp and ownership flag.
 * @param args  Keys to remove (invalid ones will simply not match anything).
 * @return int 0 on success, 1 on allocation/copy failure.
 */
static int	apply_unset(t_shell *shell, char **args)
{
	int		count;
	char	**new_env;

	count = env_count(shell->envp);
	new_env = env_alloc(count);
	if (!new_env)
		return (1);
	env_copy_filtered(shell->envp, args, new_env);
	if (shell->envp_allocated)
		ft_gcfree(CAT_ENV, shell->envp);
	shell->envp = new_env;
	shell->envp_allocated = 1;
	return (0);
}

/**
 * @brief Builtin `unset`: remove variables from the shell environment.
 *
 * Behavior:
 *  - Validates input identifiers (letters, digits, underscore;
 *  - must not start with a digit).
 *  - Prints warnings for each invalid identifier.
 *  - Detects option-like args (e.g., "-x") and returns 2 like bash does.
 *  - If there are only valid identifiers, rebuilds envp without the listed keys.
 *
 * Return codes:
 *  0 → success
 *  1 → fatal error (allocation or shell/env missing)
 *  2 → invalid option (argument starting with '-' and having at least one more char)
 *
 * @param shell Global shell state carrying envp and metadata.
 * @param args  Argument vector: args[0] = "unset", args[1..] = keys to remove.
 * @return int  See return codes above.
 */
int	built_unset(t_shell *shell, char **args)
{
	int		i;
	int		ret;

	if (!shell || !shell->envp)
		return (1);
	print_invalid_identifiers(args);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1])
			ret = 2;
		else if (!is_valid_identifier(args[i]) && ret == 0)
			ret = 1;
		i++;
	}
	if (ret == 2)
		return (2);
	if (apply_unset(shell, args) != 0)
		return (1);
	return (ret);
}
