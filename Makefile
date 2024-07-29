GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
BRIGHT_BLUE = \033[1;34m
BRIGHT_YELLOW = \033[1;33m

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -rf

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

READLINE_DIR = /opt/homebrew/opt/readline
READLINE_LIB = -L$(READLINE_DIR)/lib -lreadline

INC_DIRS = include $(LIBFT_DIR) $(READLINE_DIR)/include
CFLAGS += $(addprefix -I, $(INC_DIRS))
LDFLAGS = -L$(LIBFT_DIR) -lft $(READLINE_LIB)

SRC_DIR = src
OBJ_DIR = obj
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

DEP_FILES = $(OBJ_FILES:.o=.d)

all: tag $(NAME)

$(NAME): $(LIBFT) $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS) -o $@
	@echo "${GREEN}All source files have been successfully compiled${RESET}"
	@echo "${GREEN}Yay! Minishell is ready to run! 🚀${RESET}"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

$(LIBFT):
	@$(MAKE) -s -C $(LIBFT_DIR)
	@echo "${GREEN}Library is all set: $(LIBFT_DIR) 📚${RESET}"

clean:
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	@$(RM) $(OBJ_DIR) .tag
	@echo "${RED}Cleaned up the mess! 🧹${RESET}"

fclean: clean
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)
	@echo "${RED}All clean, like it never happened!${RESET}"

re: fclean all

-include $(DEP_FILES)

.PHONY: all clean fclean re


tag:
	@if [ ! -e .tag ]; then \
		echo "$(BRIGHT_BLUE)###############   TEAM: Ast_Monkeys   ##############"; \
		echo "$(BRIGHT_YELLOW)"; \
		echo "              _       _        __  _ _____ __    __"; \
		echo "   ____ ___  (_)___  (_)____  / / / / ___ / /   / /"; \
		echo "  / __ \`__ \/ / __ \/ / ___/ / /_/ / __/ / /   / /"; \
		echo " / / / / / / / / / / /(__  )/ __  / /___/ /___/ /___"; \
		echo "/_/ /_/ /_/_/_/ /_/_//____//_/ /_/_____/_____/_____/"; \
		echo "$(BRIGHT_BLUE)~~~~~  Alisa Arbenina  ~~~~~~~~~  Oleg Goman  ~~~~"; \
		echo "                                                   "; \
		touch .tag; \
	fi

# tag:
# 	@if [ ! -e .tag ]; then \
# 		echo "$(BRIGHT_BLUE)###############   TEAM: Ast_Monkeys   ##############"; \
# 		echo "$(BRIGHT_YELLOW)"; \
# 		echo "              _       _ _____ __  __________    __"; \
# 		echo "   ____ ___  (_)___  (_) ___// / / / ____/ /   / /"; \
# 		echo "  / __ \`__ \/ / __ \/ /\__ \/ /_/ / __/ / /   / /"; \
# 		echo " / / / / / / / / / / /___/ / __  / /___/ /___/ /___"; \
# 		echo "/_/ /_/ /_/_/_/ /_/_//____/_/ /_/_____/_____/_____/"; \
# 		echo "$(BRIGHT_BLUE)~~~~~  Alisa Arbenina  ~~~~~~~~~  Oleg Goman  ~~~~"; \
# 		echo "                                                   "; \
# 		touch .tag; \
# 	fi