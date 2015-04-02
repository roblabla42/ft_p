# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2014/11/03 11:25:08 by rlambert          #+#    #+#              #
#    Updated: 2015/04/02 23:27:32 by rlambert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS += -Wall -Wextra -Werror -Ilibft/include/ -Iinclude/

SRV_SRCS = src/server.c \
		   src/handlers_server/handle_cd.c \
		   src/handlers_server/handle_cmd.c \
		   src/handlers_server/handle_get.c \
		   src/handlers_server/handle_ls.c \
		   src/handlers_server/handle_put.c \
		   src/handlers_server/handle_pwd.c \
		   src/handlers_server/handle_quit.c \
           src/ft_stream/stream.c \
           src/ft_stream/write_stream.c \
           src/ft_stream/read_line.c

SRV_OBJS = $(SRV_SRCS:.c=.o)

CLI_SRCS = src/client.c \
		   src/handlers_client/handle_cd.c \
		   src/handlers_client/handle_get.c \
		   src/handlers_client/handle_ls.c \
		   src/handlers_client/handle_put.c \
		   src/handlers_client/handle_pwd.c \
		   src/handlers_client/handle_quit.c \
           src/ft_stream/stream.c \
           src/ft_stream/write_stream.c \
           src/ft_stream/read_line.c

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
	$(RM) $(CLI_OBJS)

fclean: clean
	$(MAKE) -C libft/ fclean
	$(RM) client serveur

re: fclean all

.PHONY: clean fclean re all
