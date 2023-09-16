# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 00:39:21 by mcesar-d          #+#    #+#              #
#    Updated: 2023/09/16 04:00:30 by pmitsuko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

## COLORS ##

DEFAULT		=	\e[39m
GREEN		=	\e[92m
YELLOW		=	\e[93m
MAGENTA		=	\e[95m
CYAN		=	\e[96m

# **************************************************************************** #

NAME = webserv

# LIBRARY #

HEADER = -I includes

# COMPILATION #

CC = c++
FLAGS = -g -std=c++98 -Werror -Wextra -Wall

# DELETE #

RM = rm -rf

# DIRECTORIES #

SRC_DIR = ./src/

VPATH = $(SRC_DIR)\
		$(SRC_DIR)server\
		$(SRC_DIR)utils

# FILES #

FILES = main.cpp\
		Socket.cpp\
		Poll.cpp\
		Server.cpp\
		Logger.cpp

# COMPILED_SOURCES #

OBJ_DIR = ./obj/
OBJS = $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

# *************************************************************************** #

# RULES #

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(HEADER) -o $(NAME)
	@echo "\n$(CYAN)----------------------------------------"
	@echo "------------- MAKE WEBSERV -------------"
	@echo "----------------------------------------\n$(DEFAULT)"

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p obj
	$(CC) $(FLAGS) $(HEADER) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	@echo "\n$(MAGENTA)----------------------------------------"
	@echo "------------- CLEANING DONE ------------"
	@echo "----------------------------------------\n$(DEFAULT)"

re: fclean all

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

run: all
	./$(NAME)

.PHONY: all clean fclean re
