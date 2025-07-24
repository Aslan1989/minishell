# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: psmolin <psmolin@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/04 12:19:44 by aisaev            #+#    #+#              #
#    Updated: 2025/07/24 13:46:08 by psmolin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror

RL_FLAGS = -lreadline

LIB_DIR = ./my_library
LIB = $(LIB_DIR)/aisaev.a

# includes from your my_library modules
INCLUDES =	-I$(LIB_DIR)/libft/includes \
			-I$(LIB_DIR)/ft_printf/includes \
			-I$(LIB_DIR)/get_next_line/includes \
			-Iinclude

SRC_DIR		= src/
OBJ_DIR		= obj/

SRC = $(SRC_DIR)main.c \
	$(SRC_DIR)prompt.c \
	$(SRC_DIR)signals.c \
	$(SRC_DIR)builtins/built_echo.c \
	$(SRC_DIR)builtins/built_unset.c \
	$(SRC_DIR)builtins/built_cd.c \
	$(SRC_DIR)builtins/built_env.c \
	$(SRC_DIR)builtins/built_exit.c \
	$(SRC_DIR)builtins/built_export.c \
	$(SRC_DIR)builtins/built_pwd.c \
	$(SRC_DIR)executor.c \
	$(SRC_DIR)find_executable.c \
	$(SRC_DIR)u_comms.c \
	$(SRC_DIR)u_env.c \
	$(SRC_DIR)u_error.c \
	$(SRC_DIR)u_exec.c \
	$(SRC_DIR)u_lib.c \
	$(SRC_DIR)u_malloc.c \
	$(SRC_DIR)u_malloc2.c \
	$(SRC_DIR)u_parse.c \
	$(SRC_DIR)u_parse2.c \
	$(SRC_DIR)u_parse3.c \
	$(SRC_DIR)u_parse4.c \
	$(SRC_DIR)u_redir.c \
	$(SRC_DIR)u_redir2.c \
	$(SRC_DIR)u_shell.c

# Changing all SRC_DIR to OBJ_DIR for objects
OBJ	= $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRC))

all: $(LIB) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) $(RL_FLAGS)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

# Creating OBJ files and creating subfolders when necessary
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re


