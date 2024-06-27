# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 07:05:18 by ogoman            #+#    #+#              #
#    Updated: 2024/04/04 08:00:59 by ogoman           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Target
NAME = pipex

# Command for copying files
CC = cc

# Header
HEADER = ./pipex.h

# Compilation flags
CFLAGS = -Wall -Wextra -Werror -g

# Command for deleting files and directories
RM = rm -rf

# Source files
SRCS = $(addprefix main_functions/, main.c process_utils.c process_creating.c errors.c)

# Helper functions
HELP_SRCS = $(addprefix helper_functions/, px_strjoin.c px_strlcpy.c px_strncmp.c \
				px_bzero.c px_calloc.c px_split.c px_strcpy.c px_strdup.c \
						px_strlen.c px_substr.c px_memset.c px_strcmp.c)

# Directory with the source files of libft
LIBFT_DIR = ./libft

# Path to the libft library and its name
LIBFT = libft/libft.a

# Object files obtained from source files
OBJS = $(SRCS:.c=.o)

# Object files obtained from help_sources files
HELP_OBJS = $(HELP_SRCS:.c=.o)

# ANSI Escape Codes
BGreen=\033[1;32m
BRed=\033[1;31m

# Default "all" target
all : tag $(LIBFT) $(NAME)

# Rule for building libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Rule for building the target file
$(NAME): $(OBJS) $(HELP_OBJS)
	@echo "\nBuilding target file: $(NAME)"
	@$(CC) $(OBJS) $(HELP_OBJS) -o $(NAME)

## Rule for building object files
%.o: %.c $(HEADER) 
	@/bin/echo -n "..."
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(HEADER)

# Rule for cleaning object files
clean:
	@echo "$(BRed)Cleaning object files"
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(RM) $(OBJS) $(HELP_OBJS)
	@echo ".......................... READY"

# Rule for full clean (including libft)
fclean:
	@echo "$(BRed)Cleaning object files and ./pipex"
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(RM) $(NAME) $(OBJS) $(HELP_OBJS)
	@if [ -e .tag ]; then \
		$(RM) .tag; \
	fi

# Rule for recompiling
re: fclean all

# Declare targets that are not real files
.PHONY: all bonus clean fclean re

# Rule for displaying the tag
tag:
	@if [ ! -e .tag ]; then \
		echo "$(BGreen)"; \
		echo "qweqwrqeqr    qweeq    qweqwrqeqr    qwewqeewqee    qwers  OG  asdfd"; \
		echo "asdfhfhsdf    dasdh    asdfhfhsdf    asgfsdfhpvs    xcbvb      asfga"; \
		echo "akj    jpq             akj    jpq    dfgh             asdf    afff"; \
		echo "jksdhksdjg    jfgsj    jksdhksdjg    pooifgmsngd        sdfsdfd"; \
		echo "pogowehoff    pogaa    jksdhksdjg    asdd             asdf    afff"; \
		echo "zxh           hddfp    jsd           asddasddffa    xcbvb      asfga"; \
		echo "tth           hddfp    zpc           asddasddffa    qwers      asdfd"; \
		touch .tag; \
	fi
