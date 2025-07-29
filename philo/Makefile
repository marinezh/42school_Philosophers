# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mzhivoto <mzhivoto@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/10 14:19:08 by mzhivoto          #+#    #+#              #
#    Updated: 2025/07/29 22:56:42 by mzhivoto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Library name
NAME = philo

COLOUR_END=\033[0m
COLOUR_BRIGHT_RED = \033[1;31m
COLOUR_BRIGHT_GREEN = \033[1;32m
COLOUR_BRIGHT_YELLOW = \033[1;33m
COLOUR_BRIGHT_BLUE = \033[1;34m
COLOUR_BRIGHT_MAGENTA = \033[1;35m
# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes/ -I./
LDFLAGS = -lpthread

SRCS_PATH = ./src
OBJS_PATH = ./obj

# Source files and object files
SRC = $(SRCS_PATH)/main.c \
		$(SRCS_PATH)/utils.c\
		$(SRCS_PATH)/parser.c\
		$(SRCS_PATH)/init.c\
		$(SRCS_PATH)/philo.c\

OBJ := $(SRC:$(SRCS_PATH)/%.c=$(OBJS_PATH)/%.o)

#OBJ := $(patsubst $(SRCS_PATH)/%.c, $(OBJS_PATH)/%.o, $(SRC))

# Default rule to create the library
.SECONDARY: $(OBJ)

all: $(NAME)

$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)
	@echo -e "$(COLOUR_BRIGHT_GREEN)obj directory created $(COLOUR_END)"

# Rule to create the library from object files
$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME) 
	@echo -e "$(COLOUR_BRIGHT_GREEN)$(NAME) created$(COLOUR_END)"

	
# Compile each .c file into a .o file
$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.c | $(OBJS_PATH)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "$(COLOUR_BRIGHT_GREEN)$@ created$(COLOUR_END)"

clean:
	@rm -rf $(OBJS_PATH)
	@echo -e "$(COLOUR_BRIGHT_RED)object directory cleaned$(COLOUR_END)"

fclean: clean
	@rm -f $(NAME)
	@echo -e "$(COLOUR_BRIGHT_RED)$(NAME) removed$(COLOUR_END)"

re: fclean all

.PHONY: all clean fclean re
