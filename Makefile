# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/25 22:43:08 by jonathanebe       #+#    #+#              #
#    Updated: 2024/08/15 20:53:33 by jeberle          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------------------------------------#
#--------------                       PRINT                       -------------#
#------------------------------------------------------------------------------#

BLACK := \033[90m
RED := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
BLUE := \033[34m
MAGENTA := \033[35m
CYAN := \033[36m
X := \033[0m

SUCCESS := \n$(BLUE)\
███████████████████████████████████████████████████████████████████$(X)\n\
$(X)\n\
███ ███  █  ██    █  █  $(BLACK)███████$(X)  █     █  ███████  █        █      $(X)\n\
█  █  █  █  █ █   █  █  $(BLACK)█$(X)        █     █  █        █        █      $(X)\n\
█  █  █  █  █  █  █  █  ███████  ███████  ███████  █        █      $(X)\n\
█     █  █  █   █ █  █        $(BLACK)█$(X)  █     █  █        █        █      $(X)\n\
█     █  █  █    ██  █  $(BLACK)███████$(X)  █     █  ███████  ███████  ███████$(X)\n\
$(X)\n\
$(BLUE)███████████████████████████████████████████████████████████████████$(X)\n\

#------------------------------------------------------------------------------#
#--------------                      GENERAL                      -------------#
#------------------------------------------------------------------------------#

NAME=minishell
NAME_BONUS=minishell_bonus

#------------------------------------------------------------------------------#
#--------------                       FLAGS                       -------------#
#------------------------------------------------------------------------------#

CC=cc
CFLAGS=-Wall -Wextra -Werror -I/usr/local/opt/readline/include
LDFLAGS=-L/usr/local/opt/readline/lib -lreadline

ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address -g
endif

DEPFLAGS=-MMD -MP

#------------------------------------------------------------------------------#
#--------------                        DIR                        -------------#
#------------------------------------------------------------------------------#

OBJ_DIR := ./obj
DEP_DIR := $(OBJ_DIR)/.deps
INC_DIRS := .
SRC_DIRS := .

vpath %.c $(SRC_DIRS)
vpath %.h $(INC_DIRS)
vpath %.d $(DEP_DIR)

#------------------------------------------------------------------------------#
#--------------                        LIBS                       -------------#
#------------------------------------------------------------------------------#

LIBFT_DIR=libft
LIBFT=libft.a
LIBFT_LIB=$(LIBFT_DIR)/$(LIBFT)
LIBFTFLAGS=-L$(LIBFT_DIR) -lft

SYSLIBFLAGS=-lreadline

#------------------------------------------------------------------------------#
#--------------                        SRC                        -------------#
#------------------------------------------------------------------------------#

SRCS=	mandatory/builtins/cd.c \
		mandatory/builtins/echo.c \
		mandatory/builtins/env.c \
		mandatory/builtins/exit.c \
		mandatory/builtins/export.c \
		mandatory/builtins/pwd.c \
		mandatory/builtins/set.c \
		mandatory/builtins/unset.c \
		mandatory/builtins/var_helper.c \
		mandatory/executer/executer_checks.c \
		mandatory/executer/executer_checks2.c \
		mandatory/executer/executer_checks3.c \
		mandatory/executer/executer_command.c \
		mandatory/executer/executer_env.c \
		mandatory/executer/executer_helper.c \
		mandatory/executer/executer_helper2.c \
		mandatory/executer/executer_inits.c \
		mandatory/executer/executer_pipes.c \
		mandatory/executer/executer_prexecute.c \
		mandatory/executer/executer_redirection.c \
		mandatory/executer/executer_runseg_helper.c \
		mandatory/executer/executer_runseg.c \
		mandatory/executer/executer_signals.c \
		mandatory/executer/executer.c \
		mandatory/expander/expand_token.c \
		mandatory/expander/expand.c \
		mandatory/expander/expand_helper.c \
		mandatory/expander/expand_heredoc.c \
		mandatory/handler/handle_heredoc.c \
		mandatory/handler/handle_infile.c \
		mandatory/handler/handle_trunc_append.c \
		mandatory/handler/handle_trunc_append2.c \
		mandatory/handler/handle_escape.c \
		mandatory/handler/handle_escape2.c \
		mandatory/handler/handle_exit_status.c \
		mandatory/handler/handle_exit_status2.c \
		mandatory/handler/handle_variable.c \
		mandatory/lexer/lexer_helper.c \
		mandatory/lexer/lexer.c \
		mandatory/lexer/prompt_to_token.c \
		mandatory/lexer/prompt_to_token2.c \
		mandatory/lexer/prompt_to_token3.c \
		mandatory/lexer/prompt_to_token4.c \
		mandatory/tokens/tokens.c \
		mandatory/tokens/tokens2.c \
		mandatory/tokens/tokens3.c \
		mandatory/tokens/tokens4.c \
		mandatory/ast.c \
		mandatory/hierarchy_validation.c \
		mandatory/init_envlst.c \
		mandatory/minishell.c \
		mandatory/minishell2.c \
		mandatory/parser.c \
		mandatory/pipes.c \
		mandatory/remove_chars.c \
		mandatory/whitespace_handler.c \
		mandatory/signals.c \

BONUS_SRCS= \
# bonus/minishell_bonus.c \

#------------------------------------------------------------------------------#
#--------------                      OBJECTS                      -------------#
#------------------------------------------------------------------------------#

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
BONUS_OBJECTS := $(addprefix $(OBJ_DIR)/, $(BONUS_SRCS:%.c=%.o))

#------------------------------------------------------------------------------#
#--------------                      COMPILE                      -------------#
#------------------------------------------------------------------------------#

.PHONY: all clean fclean re libft

all: $(LIBFT_LIB) $(NAME)

bonus: $(LIBFT_LIB) $(NAME_BONUS)

-include $(OBJECTS:.o=.d)
-include $(BONUS_OBJECTS:.o=.d)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@git submodule update --init --recursive --remote > /dev/null 2>&1
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LIBFTFLAGS) $(SYSLIBFLAGS) $(LDFLAGS)
	@echo "$(SUCCESS)"

$(NAME_BONUS): $(BONUS_OBJECTS)
	@$(CC) -o $@ $^ $(LIBFTFLAGS) $(SYSLIBFLAGS) $(LDFLAGS)
	@echo "$(SUCCESS)"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(RED)objects deleted$(X)"

fclean: clean
	@rm -rf $(NAME_BONUS)
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(RED)minishell deleted$(X)"

re: fclean all
