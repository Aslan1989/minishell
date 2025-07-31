/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/07/31 13:23:27 by psmolin          ###   ########.fr       */
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
# include "get_next_line.h"
# include <termios.h>
# include <errno.h>
# include <limits.h>
# include <sys/wait.h>
# include <fcntl.h>

# include <glob.h>

# define COLOR_R "\033[31m"
# define COLOR_G "\033[32m"
# define COLOR_Y "\033[33m"
# define COLOR_B "\033[34m"
# define COLOR_M "\033[35m"
# define COLOR_C "\033[36m"
# define COLOR_W "\033[37m"
# define COLOR_BOLD "\033[1m"
# define COLOR_X "\033[0m"

typedef enum e_token
{
	TOK_START,
	TOK_WORD,
	TOK_PIPE,
	TOK_AND,
	TOK_OR,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_EOF
}	t_etoken;

typedef enum e_gccat
{
	CAT_ENV,
	CAT_ARGS,
	CAT_TOKEN,
	CAT_CMD,
	CAT_MEM,
	CAT_MAX
}	t_egccat;

typedef enum e_redir
{
	REDIR_IN = 0,
	REDIR_OUT = 1,
	REDIR_APPEND = 2,
	REDIR_HEREDOC = 3
}	t_eredir;

typedef struct s_redir		t_redir;
typedef struct s_garbage	t_garbage;
typedef struct s_cmd		t_cmd;
typedef struct s_token		t_token;

struct s_redir
{
	int		type;
	char	*value;
	int		fd;
	t_redir	*next;
};

struct s_garbage
{
	void				*ptr;
	struct s_garbage	*next;
};

/**
 * @brief Custom structure to hold shell state.
 *
 * This structure is like a box that stores important shell-related
 * information during the program's execution. Instead of passing many
 * variables to each function, we pack everything into one structure.
 *
 * It currently contains:
 * - A copy of environment variables (envp)
 * - The last exit status of a command (used for $?)
 *
 * @typedef t_shell
 */
typedef struct s_shell
{
	char	**envp;
	int		last_exit_status;
}			t_shell;

struct s_cmd
{
	t_etoken	type;
	t_token		*token;
	t_cmd		*next_a;
	t_cmd		*next_b;
	int			isbuiltin;
	t_redir		*redir;
	int			fd_in;
	int			fd_out;
	char		**commands;
	char		*path;
	t_cmd		*parent;
	pid_t		pid;
};

struct	s_token
{
	t_etoken	type;
	char		*value;
	t_token		*next;
	t_token		*prev;
};

void		setup_signals(void);
void		disable_ctrl_echo(void);

char		*read_prompt(void);

int			built_echo(char **args);
int			built_cd(char **args);
int			built_pwd(void);
int			built_export(t_shell *shell, char **args);
int			built_unset(t_shell *shell, char **args);
int			built_env(t_shell *shell);
int			built_exit(void);

// char	**parse_input(const char *line);
int			parse_input(t_cmd *node, const char *line);

int			execute_command(t_cmd *command, t_shell *shell, char **args);
int			ft_run_commands(t_cmd *com);

char		**copy_env(char **envp);
void		free_env(char **envp);

char		*find_executable(t_shell *shell, const char *cmd);
void		setup_redirections(t_cmd *cmd);

//errors
int			ft_print_error(char *msg);

//parser_utils
int			count_args(const char *line);
char		*extract_arg(const char **line);
t_cmd		*parse_or(t_token **current);
t_cmd		*parse_and(t_token **current);
t_cmd		*parse_pipe(t_token **current);
t_cmd		*parse_word(t_token **current);
t_etoken	ft_p_check(t_token *current, t_etoken type);
t_token		*ft_p_advance(t_token **current);
t_cmd		*ft_p_add_node(t_token *token, t_cmd *next_a, t_cmd *next_b);

//free utils
int			ft_free_split(char **env);
t_shell		*get_shell(void);

//additional lib
char		*ft_strpbrk(const char *s, const char *accept);
size_t		ft_strspn(const char *s, const char *accept);
void		ft_ignore_ac_av(int argc, char **argv);

//commands
void		ft_generate_commands(char *line, t_cmd **comms);
t_cmd		*ft_parse_tokens(t_token **tokens);
void		setup_redirections(t_cmd *command);
int			ft_here_doc_input(char *limiter);
int			ft_redir_add(t_cmd *cmd, t_eredir type, char *filename);
char		**ft_expand_wildcards(char **args);

//our malloc and garbage collector
t_garbage	**get_gc(t_egccat cat);
void		free_gc_cat(t_egccat cat);
void		free_gc(void);
void		ft_gcfree(t_egccat cat, void *ptr);
void		*ft_gcmalloc(t_egccat cat, ssize_t size);
void		*ft_gcrealloc(t_egccat cat, void *ptr, ssize_t size);
char		*ft_gcstrdup(t_egccat cat, char *src);
char		*ft_gcstrndup(t_egccat cat, char *src, ssize_t n);
t_garbage	*ft_gc_addback(t_garbage **lst, void *ptr);

void		ft_print_banner(void);

#endif
