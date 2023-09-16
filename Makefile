# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 00:39:21 by mcesar-d          #+#    #+#              #
#    Updated: 2023/09/14 16:08:15 by jefernan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

RESET	:= \033[0m
CYAN	:= \033[1;36m
BLUE	:=	\e[0;31m
CHECK	:= \342\234\224
LOG		:= printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

# LIBRARY #

# COMPILATION #

CC = c++
FLAGS = -g -std=c++98 -Werror -Wextra -Wall

# DELETE #

RM = rm -rf

# DIRECTORIES #

SRC_DIR = ./

VPATH = $(SRC_DIR)\

FILES =		main \
			sources/request/Request

INCLUDE = Request.hpp
SOURCES =	$(FILES:=.cpp)

# COMPILED_SOURCES #

SRC_PATH =	./
OBJ_PATH =	obj/
SRCS =	${addprefix ${SRC_PATH}, ${SOURCES}}
OBJS =	$(addprefix $(OBJ_PATH), $(SOURCES:.cpp=.o))

# *************************************************************************** #

# RULES #

all: $(NAME)

$(NAME) : $(SRCS)
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)
	@$(LOG) " Successfully compiled! "

${OBJ_PATH}%.o:	$(SRC_PATH)%.c
	@mkdir -p obj
	@${CC} ${FLAGS} -c $< -o $@

clean:
	@$(RM) $(OBJ_PATH)
	@$(LOG) "  Objects removed!  "

fclean: clean
	@$(RM) $(NAME)
	@$(LOG) "  Program has been cleaned! "

re: fclean all

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

run: all
	./$(NAME)

.PHONY: all clean fclean re
