/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/29 01:25:24 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/31 18:11:45 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <libft.h>
#include <stdlib.h>
#include "server.h"
#include "ft_stream/stream.h"
#include "protocol.h"
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/resource.h>

int		setup_sock(struct sockaddr_in *srvaddr, int port)
{
	int	srvfd;

	if ((srvfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		ft_putendl("Socket error");
		return (-1);
	}
	int yes = 1;
	if (setsockopt(srvfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		ft_putendl("sockopt error");
		return (-1);
	}
	ft_bzero(srvaddr,sizeof(*srvaddr));
	srvaddr->sin_family = AF_INET;
	srvaddr->sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr->sin_port = htons(port);
	if (bind(srvfd,(struct sockaddr *)srvaddr,sizeof(*srvaddr)) < 0)
	{
		perror("Bind error");
		return (-1);
	}
	if (listen(srvfd, 5) < 0)
		return (-1);
	return (srvfd);
}

int		verify_cd(char *rootdir, char *cd)
{
	char	oldcwd[MAXPATHLEN];
	char	newcwd[MAXPATHLEN];

	getcwd(oldcwd, MAXPATHLEN);
	if (chdir(cd))
		return (0);
	getcwd(newcwd, MAXPATHLEN);
	if (ft_strprefix(newcwd, rootdir))
	{
		return (1);
	}
	else
	{
		if (chdir(oldcwd))
		{
			perror("chdir");
			ft_putendl(oldcwd);
		}
		return (0);
	}
}

void	send_success(t_stream *stream, char *msg)
{
	if (!write_s8(stream, ACK))
		ft_putendl("ERR");
	if (!write_string(stream, msg))
		ft_putendl("ERR");
}

void	send_failure(t_stream *stream, char *msg)
{
	if (!write_s8(stream, ERR))
		ft_putendl("ERR");
	if (!write_string(stream, msg))
		ft_putendl("ERR");
}

int		handle_cd(t_stream *stream, t_state *state)
{
	char			*to;

	if (!read_string(stream, &to))
		return (0);
	if (verify_cd(state->rootdir, to))
		send_success(stream, "CD command");
	else
		send_failure(stream, "CD command: no such file or directory");
	return (1);
}

int		run_cmd(char *cmd, char **args, int stdio[3])
{
	int				pid;
	int				status;
	struct rusage	usage;
	int				i;

	if ((pid = fork()) < 0)
	{
		perror("fork");
		return (0);
	}
	else if (pid == 0)
	{
		i = -1;
		while (++i < 3)
			if (stdio[i] != -1 && stdio[i] != i)
				dup2(stdio[i], i);
		execv(cmd, args);
		ft_putendl("EXECV FAIL");
		exit(1);
	}
	else
	{
		wait4(pid, &status, 0, &usage);
		write(stdio[1], "\0", 1);
	}
	return (1);
}

int		handle_ls(t_stream *stream, t_state *state)
{
	char			*line;
	char			**args;
	int				stdio[3];

	(void)state;
	if (!read_string(stream, &line))
		return (0);
	stdio[0] = -1;
	stdio[1] = stream->fd;
	stdio[2] = -1;
	args = ft_strsplitwith(line, " \t", NULL);
	run_cmd("/bin/ls", args, stdio);
	ft_putendl("Finished handling ls");
	return (1);
}

int		handle_pwd(t_stream *stream, t_state *state)
{
	char	cwd[MAXPATHLEN];

	(void)state;
	if (getcwd(cwd, MAXPATHLEN) == NULL)
		return (0);
	write_string(stream, cwd);
	return (1);
}

void	handle_fail(t_stream *stream, t_state *state)
{
	send_failure(stream, NULL);
	state->running = 0;
}

void	handle_cmd(int cmd, t_stream *stream, t_state *state)
{
	if (cmd == CD)
	{
		if (!handle_cd(stream, state))
			handle_fail(stream, state);
	}
	else if (cmd == LS)
	{
		if (!handle_ls(stream, state))
			handle_fail(stream, state);
	}
	/*else if (cmd == GET)
		handle_get(stream);
	else if (cmd == PUT)
		handle_put(stream);*/
	else if (cmd == PWD)
		handle_pwd(stream, state);
	else if (cmd == QUIT)
	{
		send_success(stream, "Bye");
		state->running = 0;
	}
	else
		handle_fail(stream, state);
}

void	handle(int clifd)
{
	t_stream	stream;
	int8_t		cmd;
	t_state	state;

	getcwd(state.rootdir, MAXPATHLEN);
	state.running = 1;
	state.id = clifd;
	create_stream(clifd, &stream);
	while (state.running && read_s8(&stream, &cmd))
	{
		ft_putstr("Handling cmd : ");
		ft_putchar(cmd);
		ft_putstr(" ");
		ft_putendl("");
		handle_cmd(cmd, &stream, &state);
	}
}

int		handle_cli(int srvfd, int clifd)
{
	int		pid;
	if ((pid = fork()) == 0)
	{
		ft_putendl("Connection accepted");
		close(srvfd);
		handle(clifd);
		close(clifd);
		ft_putendl("Connection closed");
		return (0);
	}
	else if (pid < 0)
		perror("fork");
	else
		close(clifd);
	return (1);
}

int		main(int argc, char **argv)
{
	int					srvfd;
	int					clifd;
	socklen_t			clilen;
	struct sockaddr_in	srvaddr;
	struct sockaddr_in	cliaddr;

	if ((srvfd = setup_sock(&srvaddr, argc > 1 ? ft_atoi(argv[1]) : 3000)) < 0)
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
		if (handle_cli(srvfd, clifd) == 0)
			return (0);
	}
	close(srvfd);
	return (0);
}
