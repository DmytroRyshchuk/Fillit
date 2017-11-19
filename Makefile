# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: omakovsk <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/10/26 18:09:32 by omakovsk          #+#    #+#              #
#    Updated: 2017/11/13 20:27:53 by omakovsk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = testfillit
FLAGS = -Wall -Werror -Wextra -c

SRC = fillit.c ft_putchar.c ft_putstr.c ft_strlen.c 

all: $(NAME)

$(NAME): 
	gcc $(FLAGS) $(SRC) $(NAME)

clean:
	@rm -f
	@echo files deleted -_-

fclean: clean
	@rm -f $(NAME)
	@echo testfillit deleted -_-

re: fclean all


.PHONY: clean fclean all re
