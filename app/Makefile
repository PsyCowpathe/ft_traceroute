# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agirona <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/10 17:07:53 by agirona           #+#    #+#              #
#    Updated: 2026/04/10 18:50:42 by agirona          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = ft_traceroute

SRCS =	main.c dns.c libft_function.c parsing.c print.c utils.c

SRCS_DIR = srcs

OBJS = $(SRCS:.c=.o)

OBJS_DIR = obj

OBJS_PATH = $(addprefix $(OBJS_DIR)/, $(OBJS))

INC	= includes

LIBFLAGS = -lm

CFLAGS	= -Wall -Wextra -Werror -I $(INC)

all: $(NAME)
	./unitTests.sh


$(NAME) :	$(OBJS_PATH)
			gcc $(CFLAGS) $(OBJS_PATH) $(LIBFLAGS) -o $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INC)/ft_traceroute.h | $(OBJS_DIR)
	gcc $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJS_PATH)

fclean:
			rm -f $(OBJS_PATH)
			rm -f $(NAME)
			rm -rf $(OBJS_DIR)

re:			fclean all

.PHONY:		all clean fclean re