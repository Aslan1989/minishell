/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 12:50:59 by aisaev            #+#    #+#             */
/*   Updated: 2025/08/19 12:32:01 by aisaev           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
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
# include <sys/stat.h>

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
	TOK_SEMI,
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
typedef struct s_arg		t_arg;

struct s_redir
{
	int		type;
	char	*value;
	int		fd;
	t_redir	*next;
};

struct s_garbage
{
	void		*ptr;
	size_t		size;
	int			owns;
	t_garbage	*next;
};

struct s_arg
{
	char	*arg;
	int		wildcard;
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
	int		is_interactive;
	int		envp_allocated;
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
	t_arg		**args;
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

// char		*read_prompt(void);
char		*read_full_prompt(void);

int			built_echo(char **args);
int			built_cd(char **args);
int			built_pwd(void);
int			built_export(t_shell *shell, char **args);
int			built_unset(t_shell *shell, char **args);
int			built_env(t_shell *shell, char **args);
int			built_exit(char **args);
// built_export.c
char		*make_env_string(const char *key, const char *value);
int			replace_env_var(char **envp, const char *key, const char *value);
// built_export_sort.c
void		print_sorted_env(char **envp);
int			is_option_arg(const char *s);

// build_cd_helpers.c
char		*target_from_oldpwd(t_shell *sh);
char		*search_in_cdpath(t_shell *sh, const char *arg);

// built_unset_helpers.c
void		env_copy_filtered(char **src, char **args, char **dst);
char		**env_alloc(int count);
int			env_count(char **envp);
int			should_remove(char *env_var, char **args);

char		*get_env_var(char **envp, const char *name);

int			add_env_var(t_shell *shell, const char *key, const char *value);
int			is_valid_identifier(const char *key);
int			parse_input(t_cmd *node, const char *line);

//executor_helpers.c
int			run_and_node(t_cmd *node);
int			run_or_node(t_cmd *node);
int			run_semi_node(t_cmd *node);
pid_t		fork_pipe_child(t_cmd *cmd, int fd, int p_fd[2], int dup_fd);
int			get_exit_status(int status);
// executor_helpers2.c
int			is_builtin(const char *s);

int			execute_command(t_cmd *command, t_shell *shell, char **args);
int			ft_run_commands(t_cmd *com);

char		**copy_env(char **envp);
char		**copy_env_cat(char **envp, t_egccat cat);
void		free_env(char **envp);

char		*find_executable(t_shell *shell, const char *cmd);

//errors
int			ft_print_error(const char *msg);
void		print_arg_err(const char *arg, const char *msg);
void		print_errno_for(const char *what);

//parser_utils
int			count_args(const char *line);
t_arg		*extract_arg(const char **line);
t_cmd		*parse_or(t_token **current);
t_cmd		*parse_and(t_token **current);
t_cmd		*parse_pipe(t_token **current);
t_cmd		*parse_word(t_token **current);
t_cmd		*parse_seq(t_token **current);
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
int			ft_isnotaword(char c);

//commands
void		ft_generate_commands(char *line, t_cmd **comms);
t_cmd		*ft_parse_tokens(t_token **tokens);
void		setup_redirections(t_cmd *command);
int			ft_here_doc_input(char *limiter);
int			ft_redir_add(t_cmd *cmd, t_eredir type, char *filename);
char		**ft_expand_wildcards(t_arg **args);

t_cmd		*parse_word_sub(t_token **current);
void		ft_open_quotes(t_arg *arg);
char		*ft_expand_env(char *str);
int			ft_char_is_good_for_env(char c);

//our malloc and garbage collector
t_garbage	**get_gc(t_egccat cat);
void		free_gc_cat(t_egccat cat);
void		free_gc(void);
void		ft_gcfree(t_egccat cat, void *ptr);
size_t		ft_gc_getsize(t_egccat cat, void *ptr);
void		*ft_gcmalloc(t_egccat cat, ssize_t size);
void		*ft_gcrealloc(t_egccat cat, void *ptr, ssize_t size);
char		*ft_gcstrdup(t_egccat cat, const char *src);
char		*ft_gcstrndup(t_egccat cat, char *src, ssize_t n);
t_garbage	*ft_gc_addback(t_garbage **lst, void *ptr);
char		*ft_gcstrjoin(t_egccat cat, const char *s1, const char *s2);

void		ft_print_banner(void);

void		parent_stdio_restore(int *save_in, int *save_out);
int			parent_redirs_apply(t_cmd *cmd, int *save_in, int *save_out);

int			ft_redir_check_next(char *next_token);

//u_redir_helpers.c
int			apply_fds_parent(int fd_in, int fd_out,
				int *save_in, int *save_out);
extern int	g_syntax_error;
void		parser_syntax_error(const char *tok);
#endif
