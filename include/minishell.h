/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/09 13:09:03 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <string.h>
# include "libft.h"
# include "ft_printf.h"
#include <termios.h>
#include <errno.h>
#include <limits.h>


/**
 * @brief Custom structure to hold shell state.
 *
 * This structure is like a box that stores important shell-related information
 * during the program's execution. Instead of passing many variables to each function,
 * we pack everything into one structure.
 *
 * It currently contains:
 * - A copy of environment variables (envp)
 * - The last exit status of a command (used for $?)
 *
 * @typedef t_shell
 */
typedef struct s_shell
{
	char **envp;	// A copy of the environment variables (like PATH, HOME, etc.)
	int last_exit_status;	// Stores the exit code of the last executed command, used for $?
} t_shell;

void setup_signals(void);
void disable_ctrl_echo(void);

char *read_prompt(void);

int built_echo(char **args);
int built_cd(char **args);
int built_pwd(void);
int built_export(t_shell *shell, char **args);
int built_unset(t_shell *shell, char **args);
int built_env(t_shell *shell);
int built_exit(void);

char **parse_input(const char *line);

int execute_command(t_shell *shell, char **args);
int handle_command(t_shell *shell, char **args);

char	**copy_env(char **envp);
void	free_env(char **envp);

char *find_executable(t_shell *shell, const char *cmd);

//parcer_utils
int	count_args(const char *line);
char	*extract_arg(const char **line);

#endif
