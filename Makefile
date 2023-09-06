# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jefernan <jefernan@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 00:39:21 by mcesar-d          #+#    #+#              #
#    Updated: 2023/09/05 20:25:38 by jefernan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

# LIBRARY #

# COMPILATION #

CC = c++
FLAGS = -g -std=c++98 -Werror -Wextra -Wall

# DELETE #

RM = rm -rf

# DIRECTORIES #

# SRC_DIR =
# INC_DIR = -I./sources/request

vpath %.cpp ./sources ./sources/request
vpath %.hpp ./sources/request

FILES =		main Request
INCLUDE = Request.hpp
SOURCES =	$(FILES:=.cpp)

# COMPILED_SOURCES #

# SRC_PATH =	./sources ./sources/request
OBJ_PATH =	./obj/
# SRCS =	${addprefix ${SRC_PATH}, ${SOURCES}}
OBJS =	$(addprefix $(OBJ_PATH), $(SOURCES:.cpp=.o))

# *************************************************************************** #

# RULES #

all: $(NAME)

$(NAME) : $(OBJS) $(OBJ_PATH)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "\e[0;35m Successfully compiled! \e[0m"

${OBJ_PATH}%.o:	%.cpp $(INCLUDE)
	@mkdir -p ${OBJ_PATH}
	@${CC} ${FLAGS} -c $< -o $@

clean:
	@$(RM) $(OBJ_PATH)
	@echo "\e[0;36m Objects removed! \e[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\e[0;36m Program has been cleaned! \e[0m"

re: fclean all

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

run: all
	./$(NAME)

.PHONY: all clean fclean re
