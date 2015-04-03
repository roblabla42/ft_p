/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 19:16:36 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 22:40:56 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include "stream.h"
#include "protocol.h"

int		send_preamble(t_stream *stream, char **cmd)
{
	int8_t	res;
	char	*resstr;

	write_s8(stream, PUT);
	write_string(stream, cmd[1], ft_strlen(cmd[1]));
	if (!read_s8(stream, &res))
		return (0);
	else if (!res)
	{
		read_string(stream, &resstr, NULL);
		ft_putendl("ERROR: ");
		ft_putendl(resstr);
		return (2);
	}
	return (1);
}

int		handle_put(t_stream *stream, char *line, char **cmd)
{
	int		newfd;
	int		size;
	char	buf[BUFSIZE];
	int		res;

	(void)line;
	if (cmd[1] == NULL || (newfd = open(cmd[1], O_RDONLY)) < 0)
		ft_putendl("Usage : put <path>");
	else
	{
		if (!(res = send_preamble(stream, cmd)))
			return (0);
		else if (res == 2)
			return (1);
		while ((size = read(newfd, buf, BUFSIZE)) > 0)
			write_string(stream, buf, size);
		write_string(stream, "", 0);
		ft_putendl("SUCCESS");
	}
	return (1);
}
