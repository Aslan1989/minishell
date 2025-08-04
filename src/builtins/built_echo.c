/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:27:21 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/31 13:47:41 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_echo(const char *str)
{
	t_shell	*sh;
	int		i;
	int		len;

	if (*str == '$')
	{
		sh = get_shell();
		if (ft_strncmp(str, "$?", 3) == 0)
			ft_printf("%d", sh->last_exit_status);
		else
		{
			i = 0;
			str++;
			while (sh->envp[i])
			{
				len = ft_strchr(sh->envp[i], '=') - sh->envp[i];
				if (ft_strncmp(sh->envp[i], str, len) == 0 && str[len] == '\0')
					return (ft_printf("%s", sh->envp[i] + len + 1), 0);
				i++;
			}
		}
	}
	else
		ft_printf("%s", str);
	return (0);
}

/**
 * @brief Implements the `echo` built-in command.
 *
 * Prints strings to the terminal. Supports the `-n` option to suppress the
 * newline.
 *
 * @param args Array of arguments. args[0] is "echo", args[1...] are strings.
 * @return int Always returns 0.
 *
 * @example
 *   echo Hello World → prints "Hello World"
 *   echo -n Hello → prints "Hello" without newline
 */
int	built_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 2)
		&& ft_strspn(args[i] + 2, "n") == ft_strlen(args[i] + 2))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_echo(args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
