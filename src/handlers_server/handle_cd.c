/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:14:20 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:01:16 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stream.h"
#include "server.h"
#include "protocol.h"
#include <stdio.h>
#include <libft.h>
#include <unistd.h>

static int	verify_cd(char *rootdir, char *cd)
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

static void	send_success(t_stream *stream, char *msg)
{
	if (!write_s8(stream, ACK))
		ft_putendl("ERR");
	if (!write_string(stream, msg, ft_strlen(msg)))
		ft_putendl("ERR");
}

int			handle_cd(t_stream *stream, t_state *state)
{
	char			*to;

	if (!read_string(stream, &to, NULL))
		return (0);
	if (verify_cd(state->rootdir, to))
		send_success(stream, "CD command");
	else
		send_failure(stream, "CD command: no such file or directory");
	return (1);
}
