# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/03 11:25:08 by rlambert          #+#    #+#              #
#    Updated: 2015/03/31 20:28:54 by roblabla         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS += -Wall -Wextra -Werror -Ilibft/include/

SRV_SRCS = server.c \
           ft_stream/stream.c \
           ft_stream/read_line.c

SRV_OBJS = $(SRV_SRCS:.c=.o)

CLI_SRCS = client.c \
           ft_stream/stream.c \
           ft_stream/read_line.c

CLI_OBJS = $(CLI_SRCS:.c=.o)

RM = rm -f

all: serveur client

libft/libft.a:
	$(MAKE) -C libft/

.PHONY: libft/libft.a

serveur: libft/libft.a $(SRV_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -Llibft/ -lft

client: libft/libft.a $(CLI_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -Llibft/ -lft

clean:
	$(MAKE) -C libft/ clean
	$(RM) $(SRV_OBJS)

fclean: clean
	$(MAKE) -C libft/ fclean
	$(RM) client server

re: fclean all
