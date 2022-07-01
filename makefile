# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/30 18:56:21 by tpereira          #+#    #+#              #
#    Updated: 2022/07/01 18:27:53 by tpereira         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

PHILO	=	philo
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -pthread -g
SRCS	=	./srcs/*.c
INC		=	-Iincludes
MAKEFLAGS += --silent

all:
	$(CC) $(CFLAGS) $(INC) $(SRCS) -o $(PHILO)
	printf "\e[38;5;46m ╔════════════════════════════════════╗\e[0m\n"
	printf "\e[38;5;46m█▓▒░	      Philo Project         ░▒▓█ \e[0m\n"
	printf "\e[38;5;46m█▓▒░		Compiled	    ░▒▓█ \e[0m\n"
	printf "\e[38;5;46m ╚════════════════════════════════════╝\e[0m\n"

cc:	all
	time ./philo

clean :
	rm -rf ./srcs/*.o
	printf "\e[31;5;200m ╔════════════════════════════════════╗\e[0m\n"
	printf "\e[31;5;200m░▒▓█         Clean complete  	    █▓▒░\e[0m\n"
	printf "\e[31;5;200m ╚════════════════════════════════════╝\e[0m\n"

fclean : clean
	@rm -rf $(PHILO)

re :	fclean all
bonus:	all

.PHONY: bonus re fclean clean all