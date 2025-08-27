/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:19:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/26 19:02:26 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if the command name is a builtin.
 * @param s Command name (args[0]).
 * @return int 1 if builtin, else 0.
 */
int	is_builtin(const char *s)
{
	if (!s)
		return (0);
	if (!ft_strcmp(s, "cd"))
		return (1);
	if (!ft_strcmp(s, "export"))
		return (1);
	if (!ft_strcmp(s, "unset"))
		return (1);
	if (!ft_strcmp(s, "exit"))
		return (1);
	if (!ft_strcmp(s, "echo"))
		return (1);
	if (!ft_strcmp(s, "pwd"))
		return (1);
	if (!ft_strcmp(s, "env"))
		return (1);
	return (0);
}

/**
 * @brief Normalize waitpid status into shell exit status.
 * @param status Value returned by waitpid.
 * @return int Exit code.
 */
int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	precheck_command(t_cmd *command, t_shell *shell, char **args)
{
	if (!args || !args[0] || args[0][0] == '\0')
		return (127);
	if (find_command(command, shell, args) != 0)
		return (127);
	return (0);
}
