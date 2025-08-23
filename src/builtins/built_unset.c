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
 * @brief Builtin: unset
 *
 * Remove keys from the environment if identifiers are valid.
 *
 * @param shell Shell state.
 * @param args Keys to remove (args[1..]).
 * @return int 0 on success, 1 on fatal allocation errors.
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
