/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:14:20 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 17:25:50 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stream.h"
#include "server.h"
#include "protocol.h"
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
		return (1);
	else
	{
		if (chdir(oldcwd))
		{
			ft_putendl("Failed to chdir");
			return (-1);
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
	int				res;

	if (!read_string(stream, &to, NULL))
		return (0);
	if (!(res = verify_cd(state->rootdir, to)))
		send_failure(stream, "CD command: no such file or directory");
	else if (res == -1)
	{
		send_failure(stream, "CD command: internal error");
		return (0);
	}
	else
		send_success(stream, "CD command");
	return (1);
}
