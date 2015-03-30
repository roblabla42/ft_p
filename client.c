/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 18:49:55 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/30 19:31:32 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <arpa/inet.h>
#include <libft.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include "ft_stream/stream.h"
#include "protocol.h"

int		setup_sock(struct sockaddr_in *srvaddr, char *host, int port)
{
	int	clifd;
	struct hostent	*server;

	server = gethostbyname(host);
	if ((clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (-1);
	ft_bzero(srvaddr, sizeof(*srvaddr));
	srvaddr->sin_family = AF_INET;
	ft_memmove(server->h_addr, &srvaddr->sin_addr.s_addr, server->h_length);
	srvaddr->sin_port = htons(port);
	if (connect(clifd, (struct sockaddr*)srvaddr, sizeof(*srvaddr)) < 0)
		return (-1);
	else
		return (clifd);
}

int		handle_line(char *line, t_stream *stream)
{
	char	**cmd;
	cmd = ft_strsplitwith(line, " \t", NULL);
	if (cmd[0] == NULL)
		return (0);
	if (ft_strequ(cmd[0], "cd"))
	{
		write_s8(stream, CD);
		write_string(stream, cmd[1]);
	}
	else if (ft_strequ(cmd[0], "ls"))
	{

	}
	else if (ft_strequ(cmd[0], "get"))
	{

	}
	else if (ft_strequ(cmd[0], "put"))
	{

	}
	else if (ft_strequ(cmd[0], "pwd"))
	{

	}
	else if (ft_strequ(cmd[0], "quit"))
	{

	}
	else
	{
		ft_putendl("Unknown command");
	}
	// TODO : Mass free
	return (0);
}

int		main(int argc, char **argv)
{
	int					clifd;
	t_stream			in;
	t_stream			stream;
	struct sockaddr_in	srvaddr;
	char				*line;

	(void)argc;
	(void)argv;
	if ((clifd = setup_sock(&srvaddr, "localhost", 3000)) < 0)
	{
		ft_putendl("An error has occured while creating the client");
		return (1);
	}
	ft_putendl("Connecting to port 3000");
	create_stream(1, &in);
	create_stream(clifd, &stream);
	while (read_line(&in, &line))
	{
		handle_line(line, &stream);
		free(line);
	}
	close(clifd);
	return (0);
}
