# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/09 13:35:48 by ghambrec          #+#    #+#              #
#    Updated: 2025/01/16 12:47:45 by ghambrec         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---------- MAIN ---------- #
NAME = pipex
CC = cc
INCLUDE_DIR = ./include
LIBFT_DIR = ./sources/myLibft
LIBFT_NAME = libft.a
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR)

# ---------- SOURCES ---------- #
VPATH = ./sources
SOURCES =	main.c helper.c

# ---------- OBJECTS ---------- #
OBJECT_DIR = objects
OBJECTS = $(addprefix $(OBJECT_DIR)/, $(SOURCES:.c=.o))
OBJECTS_BONUS = $(addprefix $(OBJECT_DIR)/, $(SOURCES_BONUS:.c=.o))

# ---------- COLORS AND STUFF ---------- #
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
CYAN = \033[0;36m
NC = \033[0m

# ---------- RULES ---------- #
all: $(NAME)

$(NAME): checkMyLibft $(LIBFT_NAME) $(OBJECTS)
	@echo "$(YELLOW)Compiling $(NAME)$(NC)"
	@cc $(CFLAGS) $(OBJECTS) $(LIBFT_DIR)/$(LIBFT_NAME) -o $(NAME)
	@if [ -f $(NAME) ]; then \
		echo "$(CYAN)--------------------------------------$(NC)"; \
		echo "$(GREEN)BUILD PROCESS COMPLETED SUCCESSFULLY!$(NC)"; \
		echo "$(CYAN)--------------------------------------$(NC)"; \
	else \
		echo "$(RED)failed to compile $(NAME)$(NC)"; \
		exit 1; \
	fi

checkMyLibft:
	@if [ -d $(LIBFT_DIR) ]; then \
		echo "$(GREEN)myLibft exists$(NC)"; \
	else \
		curl -L -o $(LIBFT_DIR).zip https://github.com/ghambrec/42-myLibft/archive/refs/heads/master.zip &> /dev/null; \
		unzip -o $(LIBFT_DIR).zip &> /dev/null; \
		mv 42-myLibft-master $(LIBFT_DIR); \
		rm $(LIBFT_DIR).zip; \
		if [ -d $(LIBFT_DIR) ]; then \
			echo "$(GREEN)myLibft exists$(NC)"; \
		else \
			echo "$(RED)Error downloading myLibft$(NC)"; \
			exit 1; \
		fi \
	fi

# test rule for faster testing (without compiling myLibft again)
test: $(OBJECTS)
	@echo "$(YELLOW)\nCompiling $(NAME)$(NC)"
	@cc $(CFLAGS) $(OBJECTS) $(LIBFT_DIR)/$(LIBFT_NAME) -o $(NAME)
	@if [ -f $(NAME) ]; then \
		echo "$(CYAN)--------------------------------------$(NC)"; \
		echo "$(GREEN)BUILD PROCESS COMPLETED SUCCESSFULLY!$(NC)"; \
		echo "$(CYAN)--------------------------------------$(NC)"; \
	else \
		echo "$(RED)failed to compile $(NAME)$(NC)"; \
		exit 1; \
	fi

# exec test without flags
testnf: CFLAGS = -I $(INCLUDE_DIR)
testnf: test

$(LIBFT_NAME):
	@echo "$(YELLOW)Creating $(LIBFT_NAME)$(NC)"
	@make re -C $(LIBFT_DIR) > $(LIBFT_DIR)/make_log.txt
	@if [ -f $(LIBFT_DIR)/$(LIBFT_NAME) ]; then \
		echo "$(GREEN)$(LIBFT_NAME) successfully created$(NC)"; \
	else \
		echo "$(RED)failed to compile $(LIBFT_DIR)$(NC)"; \
		exit 1; \
	fi

$(OBJECT_DIR)/%.o: %.c | $(OBJECT_DIR)
	@printf "$(YELLOW)Compiling $(CYAN)$<\n$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJECT_DIR):
	@echo "$(YELLOW)Creating $(OBJECT_DIR) directory$(NC)"
	@mkdir -p $(OBJECT_DIR)

bonus: $(LIBFT_NAME) $(OBJECTS_BONUS)
	@echo "$(YELLOW)Compiling bonus $(NAME_CHECKER)$(NC)"
	@cc $(CFLAGS) $(OBJECTS_BONUS) $(LIBFT_DIR)/$(LIBFT_NAME) -o $(NAME_CHECKER)
	@if [ -f checker ]; then \
		echo "$(CYAN)--------------------------------------$(NC)"; \
		echo "$(GREEN)BONUS PROCESS COMPLETED SUCCESSFULLY!$(NC)"; \
		echo "$(CYAN)--------------------------------------$(NC)"; \
	else \
		echo "$(RED)failed to compile bonus$(NAME)$(NC)"; \
		exit 1; \
	fi

clean:
	@echo "$(RED)Cleaning object files$(NC)"
	@rm -rf $(OBJECT_DIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)$(NC)"
	@rm -f $(NAME)
	@rm -f $(NAME_CHECKER)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re test testnf