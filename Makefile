# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aloubier <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/24 12:58:01 by aloubier          #+#    #+#              #
#    Updated: 2023/04/28 18:59:48 by aloubier         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
SRCDIR = src
INCDIR = includes
OBJ_DIR = build

SRC = $(SRCDIR)/main.c $(SRCDIR)/pipex_utils.c
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJ_DIR)/%.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

all: $(NAME)

bonus: all

$(NAME): $(OBJ)
	make all -C libft
	$(CC) -o $(NAME) $(OBJ) libft/libft.a

$(OBJ_DIR)/%.o:	$(SRCDIR)/%.c
	mkdir -p '$(@D)'
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make clean -C libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
