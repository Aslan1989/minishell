# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/04 12:19:44 by aisaev            #+#    #+#              #
#    Updated: 2025/07/09 13:09:31 by aisaev           ###   ########.fr        #
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

SRC = src/main.c src/prompt.c src/signals.c src/builtins/built_echo.c \
	src/builtins/built_unset.c src/builtins/built_cd.c src/builtins/built_env.c \
	src/builtins/built_exit.c src/builtins/built_export.c src/builtins/built_pwd.c \
	src/parcer.c src/executor.c src/env_utils.c src/find_executable.c \
	src/parcer_utils.c
OBJ = $(SRC:.c=.o)

all: $(LIB) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) $(RL_FLAGS)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

.PHONY: all clean fclean re


