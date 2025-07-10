# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aisaev <aisaev@student.42heilbronn.de>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/12 17:34:39 by aisaev            #+#    #+#              #
#    Updated: 2025/07/09 13:23:59 by aisaev           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

FT_PRINTF_DIR = ft_printf
GNL_DIR = get_next_line
LIBFT_DIR = libft

FT_PRINTF_SRCS_DIR = $(FT_PRINTF_DIR)/src
FT_PRINTF_OBJS_DIR = $(FT_PRINTF_DIR)/obj
FT_PRINTF_INC_DIR = $(FT_PRINTF_DIR)/includes

GNL_SRCS_DIR = $(GNL_DIR)/src
GNL_OBJS_DIR = $(GNL_DIR)/obj
GNL_INC_DIR = $(GNL_DIR)/includes

LIBFT_SRCS_DIR = $(LIBFT_DIR)/src
LIBFT_OBJS_DIR = $(LIBFT_DIR)/obj
LIBFT_INC_DIR = $(LIBFT_DIR)/includes

INCLUDE = -I$(FT_PRINTF_INC_DIR) -I$(GNL_INC_DIR) -I$(LIBFT_INC_DIR)

NAME = aisaev.a

SRC_FT_PRINTF = ft_printf.c ft_putchar.c ft_puthex.c ft_putnbr.c ft_putpointer.c \
ft_putstr.c ft_putunsigned.c

FT_PRINTF_SRCS = $(addprefix $(FT_PRINTF_SRCS_DIR)/, $(SRC_FT_PRINTF))

SRC_GET_NEXT_LINE = get_next_line.c get_next_line_utils.c

GNL_SRCS = $(addprefix $(GNL_SRCS_DIR)/, $(SRC_GET_NEXT_LINE))

SRC_LIBFT = ft_strlen.c ft_isalpha.c ft_isdigit.c ft_isalnum.c \
ft_isprint.c ft_isascii.c ft_memset.c ft_memcpy.c ft_memmove.c \
ft_atoi.c ft_bzero.c ft_calloc.c ft_itoa.c ft_memchr.c ft_memcmp.c \
ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c \
ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
ft_strlcpy.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_strcmp.c ft_isspace.c

LIBFT_SRCS := $(addprefix $(LIBFT_SRCS_DIR)/, $(SRC_LIBFT))

FT_PRINTF_OBJS = $(FT_PRINTF_SRCS:$(FT_PRINTF_SRCS_DIR)/%.c=$(FT_PRINTF_OBJS_DIR)/%.o)
GNL_OBJS = $(GNL_SRCS:$(GNL_SRCS_DIR)/%.c=$(GNL_OBJS_DIR)/%.o)
LIBFT_OBJS = $(LIBFT_SRCS:$(LIBFT_SRCS_DIR)/%.c=$(LIBFT_OBJS_DIR)/%.o)

OBJS = $(FT_PRINTF_OBJS) $(GNL_OBJS) $(LIBFT_OBJS)


all: $(NAME)

$(FT_PRINTF_OBJS_DIR)/%.o: $(FT_PRINTF_SRCS_DIR)/%.c | $(FT_PRINTF_OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(GNL_OBJS_DIR)/%.o: $(GNL_SRCS_DIR)/%.c | $(GNL_OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFT_OBJS_DIR)/%.o: $(LIBFT_SRCS_DIR)/%.c | $(LIBFT_OBJS_DIR)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(FT_PRINTF_OBJS_DIR):
	@mkdir -p $(FT_PRINTF_OBJS_DIR)

$(GNL_OBJS_DIR):
	@mkdir -p $(GNL_OBJS_DIR)

$(LIBFT_OBJS_DIR):
	@mkdir -p $(LIBFT_OBJS_DIR)

#compile the library
$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

bonus: all

clean:
	@$(RM) $(FT_PRINTF_OBJS_DIR) $(GNL_OBJS_DIR) $(LIBFT_OBJS_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all bonus

