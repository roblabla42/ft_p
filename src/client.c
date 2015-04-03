/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/01 20:22:44 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 22:40:00 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <libft.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "stream.h"
#include "client.h"
#include "protocol.h"

int		setup_sock(char *host, int port)
{
	int					clifd;
	struct sockaddr_in	srvaddr;
	struct hostent		*server;

	server = gethostbyname(host);
	if ((clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (-1);
	ft_bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	ft_memmove(server->h_addr, &srvaddr.sin_addr.s_addr, server->h_length);
	srvaddr.sin_port = htons(port);
	if (connect(clifd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
		return (-1);
	else
		return (clifd);
}

int		handle_line(char *line, t_stream *stream, int *running)
{
	char	**cmd;

	cmd = ft_strsplitwith(line, " \t", NULL);
	if (cmd[0] == NULL)
		return (1);
	else if (ft_strequ(cmd[0], "cd"))
		return (handle_cd(stream, line, cmd));
	else if (ft_strequ(cmd[0], "ls"))
		return (handle_ls(stream, line, cmd));
	else if (ft_strequ(cmd[0], "get"))
		return (handle_get(stream, line, cmd));
	else if (ft_strequ(cmd[0], "put"))
		return (handle_put(stream, line, cmd));
	else if (ft_strequ(cmd[0], "pwd"))
		return (handle_pwd(stream, line, cmd));
	else if (ft_strequ(cmd[0], "quit"))
		return (handle_quit(stream, line, cmd, running));
	else
		ft_putendl("Unknown command");
	return (1);
}

void	do_loop(t_stream *stream, t_stream *in)
{
	char	*line;
	int		running;

	running = 1;
	while (running && read_until(in, &line, '\n') > 0
				&& handle_line(line, stream, &running))
	{
		free(line);
		ft_putstr("ft_p> ");
	}
	if (running)
		ft_putendl("ERROR: Protocol Error");
}

int		main(int argc, char **argv)
{
	int					clifd;
	t_stream			in;
	t_stream			stream;

	if (argc < 3)
	{
		ft_putendl("Usage: ./client <machine> <port>");
		return (1);
	}
	if ((clifd = setup_sock(argv[1], ft_atoi(argv[2]))) < 0)
	{
		ft_putendl("An error has occured while creating the client");
		return (1);
	}
	ft_putstr("Connecting to port 3000\nft_p> ");
	stream = (t_stream){ .fd = clifd, .size = 1, .cursor = 1 };
	create_stream(1, &in);
	do_loop(&stream, &in);
	close(clifd);
	return (0);
}
