# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmitsuko <pmitsuko@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/04 00:39:21 by mcesar-d          #+#    #+#              #
#    Updated: 2023/10/03 22:11:06 by pmitsuko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CHECK		=	\342\234\224

## COLORS ##

RESET		=	\e[39m
GREEN		=	\e[92m
YELLOW		=	\e[93m
MAGENTA		=	\e[95m
CYAN		=	\e[96m
LOG			= printf "[$(CYAN)$(CHECK)$(RESET)] %s\n"

# **************************************************************************** #

NAME = webserv

# LIBRARY #

HEADER = -I includes

# COMPILATION #

CC = c++
FLAGS = -g3 -std=c++98 -Werror -Wextra -Wall

# DELETE #

RM = rm -rf

# DIRECTORIES #

SRC_DIR = ./src/

VPATH = $(SRC_DIR)\
		$(SRC_DIR)location\
		$(SRC_DIR)parser\
		$(SRC_DIR)request\
		$(SRC_DIR)server\
		$(SRC_DIR)utils\
# FILES #

FILES = main.cpp\
		Socket.cpp\
		Poll.cpp\
		Server.cpp\
		Logger.cpp\
		Parser.cpp\
		Request.cpp\
		Location.cpp\


# COMPILED_SOURCES #

OBJ_DIR = ./obj/
OBJS = $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

# *************************************************************************** #

# RULES #

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(HEADER) -o $(NAME)
	@$(LOG) " Successfully compiled! "

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p obj
	$(CC) $(FLAGS) $(HEADER) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)
	@$(LOG) "  Objects removed!  "

fclean: clean
	$(RM) $(NAME)
	@$(LOG) "  Program has been cleaned! "

re: fclean all

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) ./config/default.conf

run: all
	./$(NAME)

.PHONY: all clean fclean re
