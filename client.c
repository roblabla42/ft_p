/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 18:49:55 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/31 18:38:17 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <arpa/inet.h>
#include <libft.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
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

int		read_result(t_stream *stream)
{
	int8_t	cmd;
	char	*reason;

	if (!read_s8(stream, &cmd))
		return (0);
	if (cmd == ACK || cmd == ERR)
	{
		ft_putstr(cmd == ACK ? "SUCCESS :" : "ERROR :");
		if (!read_string(stream, &reason))
			return (0);
		else
			ft_putendl(reason);
	}
	else
	{
		ft_putendl("PROTOCOL ERROR");
		return (0);
	}
	return (1);
}

int		handle_line(char *line, t_stream *stream)
{
	char	**cmd;
	char	*lsres;

	cmd = ft_strsplitwith(line, " \t", NULL);
	if (cmd[0] == NULL)
		return (1);
	else if (ft_strequ(cmd[0], "cd"))
	{
		write_s8(stream, CD);
		write_string(stream, cmd[1]);
		return (read_result(stream));
	}
	else if (ft_strequ(cmd[0], "ls"))
	{
		write_s8(stream, LS);
		write_string(stream, line);
		if (!read_until(stream, &lsres, '\0'))
			return (0);
		write(1, lsres, ft_strlen(lsres));
	}
	else if (ft_strequ(cmd[0], "get"))
	{
		if (cmd[1] == NULL)
			ft_putendl("Usage : get <path>");
		else
		{
			write_s8(stream, GET);
			write_string(stream, cmd[1]);
			int newfile = open(cmd[1], O_WRONLY);
			while (read_string(stream, &lsres) && ft_strlen(lsres) != 0)
				write(newfile, lsres, ft_strlen(lsres));
			close(newfile);
		}
	}
	else if (ft_strequ(cmd[0], "put"))
	{
		write_s8(stream, PUT);
	}
	else if (ft_strequ(cmd[0], "pwd"))
	{
		write_s8(stream, PWD);
		if (!read_string(stream, &lsres))
			return (0);
		ft_putendl(lsres);
	}
	else if (ft_strequ(cmd[0], "quit"))
	{
		write_s8(stream, QUIT);
		read_result(stream);
		return (0);
	}
	else
	{
		ft_putendl("Unknown command");
	}
	// TODO : Mass free
	return (1);
}

int		main(int argc, char **argv)
{
	int					clifd;
	t_stream			in;
	t_stream			stream;
	struct sockaddr_in	srvaddr;
	char				*line;
	int					res;

	(void)argc;
	(void)argv;
	if ((clifd = setup_sock(&srvaddr, "localhost", 3000)) < 0)
	{
		ft_putendl("An error has occured while creating the client");
		return (1);
	}
	ft_putendl("Connecting to port 3000");
	ft_putstr("> ");
	create_stream(1, &in);
	stream.fd = clifd;
	stream.size = 1;
	stream.cursor = 1;
	while ((res = read_until(&in, &line, '\n')) > 0
			&& handle_line(line, &stream))
	{
		free(line);
		ft_putstr("> ");
	}
	if (res > 0)
		ft_putendl("Connection reset by peer");
	close(clifd);
	return (0);
}
