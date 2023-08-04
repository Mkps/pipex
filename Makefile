# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aloubier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/08/04 07:15:42 by aloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
BONUS = pipex_bonus
TARGET = pipex
SRCDIR = src
INCDIR = includes
OBJ_DIR = build

SRC = $(SRCDIR)/main.c $(SRCDIR)/pipex_utils.c $(SRCDIR)/here_doc.c \
		$(SRCDIR)/pipex_cmd.c $(SRCDIR)/child.c $(SRCDIR)/pipex.c
SRC_BONUS = $(SRCDIR)/main_bonus.c $(SRCDIR)/pipex_utils_bonus.c $(SRCDIR)/here_doc_bonus.c \
			$(SRCDIR)/pipex_cmd_bonus.c $(SRCDIR)/child_bonus.c $(SRCDIR)/pipex_bonus.c
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_BONUS = $(SRC_BONUS:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling objects for mandatory part"
	@make -s all -C libft
	$(CC) -o $(TARGET) $(OBJ) libft/libft.a
	
bonus: $(OBJ_BONUS)
	@echo "Compiling objects for bonus part"
	@make -s all -C libft
	$(CC) -o $(TARGET) $(OBJ_BONUS) libft/libft.a

$(OBJ_DIR)/%.o:	$(SRCDIR)/%.c
	@mkdir -p '$(@D)'
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@make -s clean -C libft

fclean: clean
	@echo "Cleaning binaries..."
	@rm -f $(NAME)
	@make -s fclean -C libft

re: fclean all

rebonus: fclean bonus

.PHONY: all clean fclean re bonus rebonus
