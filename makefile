# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/30 18:56:21 by tpereira          #+#    #+#              #
#    Updated: 2022/08/22 19:56:01 by tpereira         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -pthread -g -Ofast
SRCS	=	./srcs/*.c
INC		=	./includes/*.h
MAKEFLAGS += --silent

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)
	printf "\e[38;5;46m ╔════════════════════════════════════╗\e[0m\n"
	printf "\e[38;5;46m█▓▒░	      Philo Project         ░▒▓█ \e[0m\n"
	printf "\e[38;5;46m█▓▒░		Compiled	    ░▒▓█ \e[0m\n"
	printf "\e[38;5;46m ╚════════════════════════════════════╝\e[0m\n"

cc:	all
	./philo 3 1 2 3

clean :
	rm -rf ./srcs/*.o
	printf "\e[31;5;200m ╔════════════════════════════════════╗\e[0m\n"
	printf "\e[31;5;200m░▒▓█         Clean complete  	    █▓▒░\e[0m\n"
	printf "\e[31;5;200m ╚════════════════════════════════════╝\e[0m\n"

fclean : clean
	@rm -rf $(NAME)

re :	fclean all
bonus:	all

.PHONY: bonus re fclean clean all