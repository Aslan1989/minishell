/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:36:40 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/28 15:55:57 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the string is a syntactic integer: [+|-] and then digits only.
 *
 * Accepts an optional leading '+' or '-', followed by at least one digit.
 * No whitespace is allowed and no overflow checks are performed.
 *
 * @param str  Input string.
 * @return int
 * @retval 1 if syntactically numeric.
 * @retval 0 otherwise (NULL, empty, sign-only, or contains non-digits).
 */
int	ft_is_numeric(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static void	print_too_many_args(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
}

static void	print_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

static void	cleanup_and_exit(int code)
{
	free_gc();
	rl_clear_history();
	exit(code);
}

/**
 * @brief Builtin: exit
 *
 * Behaviour:
 * - No args: exit(0)
 * - One non-numeric: print error, exit(255)
 * - One numeric: exit((unsigned char)code)
 * - More than one arg: print error and return 1 without exiting.
 *
 * @param args Command arguments.
 * @return int 1 on "too many arguments", otherwise does not return.
 */
int	built_exit(char **args)
{
	long	code;

	if (!args[1])
	{
		if (get_shell()->is_interactive)
			ft_putendl_fd("exit", STDOUT_FILENO);
		cleanup_and_exit(0);
	}
	if (!ft_is_numeric(args[1]))
	{
		if (get_shell()->is_interactive)
			ft_putendl_fd("exit", STDOUT_FILENO);
		print_numeric_error(args[1]);
		cleanup_and_exit(255);
	}
	if (args[2])
		return (print_too_many_args(), 1);
	code = ft_atoi(args[1]);
	if (get_shell()->is_interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	cleanup_and_exit((unsigned char)code);
	return (0);
}
