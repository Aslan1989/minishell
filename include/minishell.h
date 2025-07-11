/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/11 13:40:33 by psmolin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <string.h>
# include "libft.h"
# include "ft_printf.h"
# include <termios.h>
# include <errno.h>
# include <limits.h>
# include <sys/wait.h>


# define COLOR_R "\033[31m"
# define COLOR_G "\033[32m"
# define COLOR_Y "\033[33m"
# define COLOR_B "\033[34m"
# define COLOR_M "\033[35m"
# define COLOR_C "\033[36m"
# define COLOR_W "\033[37m"
# define COLOR_BOLD "\033[1m"
# define COLOR_X "\033[0m"

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
	char	**envp;	// A copy of the environment variables (like PATH, HOME, etc.)
	int		last_exit_status;	// Stores the exit code of the last executed command, used for $?
} t_shell;

void	setup_signals(void);
void	disable_ctrl_echo(void);

char	*read_prompt(void);

int		built_echo(char **args);
int		built_cd(char **args);
int		built_pwd(void);
int		built_export(t_shell *shell, char **args);
int		built_unset(t_shell *shell, char **args);
int		built_env(t_shell *shell);
int		built_exit(char **args);

char	**parse_input(const char *line);

int		execute_command(t_shell *shell, char **args);
int		handle_command(t_shell *shell, char **args);

char	**copy_env(char **envp);
void	free_env(char **envp);

char	*find_executable(t_shell *shell, const char *cmd);

//parcer_utils
int		count_args(const char *line);
char	*extract_arg(const char **line);

//free utils
int		ft_free_split(char **env);
t_shell	*get_shell(void);

#endif
