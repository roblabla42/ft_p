/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:18:54 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/04 06:55:30 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <libft.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdlib.h>

static int	run_cmd(char *cmd, char **args, int stdio[3])
{
	int				pid;
	int				status;
	struct rusage	usage;
	int				i;

	if ((pid = fork()) < 0)
		return (0);
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
		wait4(pid, &status, 0, &usage);
	return (1);
}

int			handle_ls(t_stream *stream, t_state *state)
{
	char			*line;
	char			**args;
	int				stdio[3];
	int				i;

	(void)state;
	if (!read_string(stream, &line, NULL))
		return (0);
	stdio[0] = -1;
	stdio[1] = stream->fd;
	stdio[2] = stream->fd;
	args = ft_strsplitwith(line, " \t", NULL);
	i = 1;
	while (args[i] != NULL)
	{
		if (ft_strstr(args[i], "..") || args[i][0] == '/')
			args[i] = "";
		i++;
	}
	run_cmd("/bin/ls", args, stdio);
	write(stdio[1], "\0", 1);
	return (1);
}
