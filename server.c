/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/29 01:25:24 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/30 19:30:00 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <libft.h>
#include "ft_stream/stream.h"
#include "protocol.h"

int		setup_sock(struct sockaddr_in *srvaddr)
{
	int	srvfd;

	if ((srvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ft_putendl("Socket error");
		return (-1);
	}
	ft_bzero(srvaddr,sizeof(*srvaddr));
	srvaddr->sin_family = AF_INET;
	srvaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr->sin_port = htons(3000);
	if (bind(srvfd,(struct sockaddr *)srvaddr,sizeof(*srvaddr)) < 0)
	{
		perror("Bind error");
		return (-1);
	}
	/*ft_bzero(srvaddr, sizeof(*srvaddr));
	srvaddr->sin_family = AF_INET;
	srvaddr->sin_port = htons(3000); // TODO : Take from argv[1]
	srvaddr->sin_addr.s_addr = INADDR_ANY;
	if (bind(srvfd, (struct sockaddr*)&srvaddr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("Bind error");
		return (-1);
	}*/
	if (listen(srvfd, 5) < 0)
		return (-1);
	return (srvfd);
}

int		verify_cd(char *cd)
{
	//size_t	i;

	//i = 0;
	return (!ft_strprefix("..", cd));
}

void	send_success(t_stream *stream, char *msg)
{
	write_s8(stream, ACK);
	write_string(stream, msg);
}

void	send_fail(t_stream *stream, char *msg)
{
	write_s8(stream, ERR);
	write_string(stream, msg);
}

void	handle_cd(t_stream *stream)
{
	char			*to;
	//unsigned int	size;

	if (!read_string(stream, &to))
		// TODO : do stuff
	if (verify_cd(to))
		chdir(to);
	send_success(stream, NULL);
}

void	handle_fail(t_stream *stream)
{
	send_fail(stream, NULL);
	close(stream->fd);
}

void	handle_cmd(int cmd, t_stream *stream)
{
	if (cmd == CD)
		handle_cd(stream);
	/*else if (cmd == 'l')
		handle_ls(stream);
	else if (cmd == 'g')
		handle_get(stream);
	else if (cmd == 'p')
		handle_put(stream);
	else if (cmd == 'w')
		handle_pwd(stream);
	else if (cmd == 'q')
		handle_quit(stream);*/
	else
		handle_fail(stream);
}

void	handle(int clifd)
{
	t_stream	stream;
	int8_t		cmd;

	create_stream(clifd, &stream);
	while (read_s8(&stream, &cmd))
	{
		handle_cmd(cmd, &stream);
	}
}

int		main(int argc, char **argv)
{
	int					srvfd;
	int					clifd;
	socklen_t			clilen;
	struct sockaddr_in	srvaddr;
	struct sockaddr_in	cliaddr;
	int					pid;

	(void)argc;
	(void)argv;
	if ((srvfd = setup_sock(&srvaddr)) < 0)
	{
		ft_putendl("An error has occured while creating the server");
		return (1);
	}
	ft_putendl("Listening on port 3000");
	while (1)
	{
		clilen = sizeof(cliaddr);
		if ((clifd = accept(srvfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
		{
			perror("accept");
			return (1);
		}
		if ((pid = fork()) == 0)
		{
			ft_putendl("Connection accepted");
			close(srvfd);
			handle(clifd);
			close(clifd);
			ft_putendl("Connection closed");
			return (0);
		}
		else
		{
			if (pid < 0)
			{
				perror("fork");
				return (1);
			}
			else
				close(clifd);
		}
	}
	close(srvfd);
	return (0);
}
