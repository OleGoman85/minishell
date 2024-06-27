# Executable name
NAME = proverka

# Compilation
CC = cc

RM = rm -rf

# Flags
CFLAGS = -Wall -Wextra -Werror -g

# List of files
SRC_FILES = proverka.c 
			# mini_cd.c \
			# mini_echo_c mini_pwd.c \
			# mini_export.c 

all: $(NAME)

# Building the executable directly from the source file
$(NAME):
	@$(CC) $(CFLAGS) -o $(NAME) $(SRC_FILES)

clean:
	rm -f $(NAME)

fclean:
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean
