/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:17:41 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/04 02:04:19 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include <sys/stat.h>

int		verify(int fd)
{
	struct stat	buf;

	if (fstat(fd, &buf))
		return (0);
	return (!S_ISDIR(buf.st_mode));
}

int		handle_get(t_stream *stream, t_state *state)
{
	int		fd;
	char	*line;
	char	block[4096];
	ssize_t	readres;

	(void)state;
	if (!read_string(stream, &line, NULL))
		return (0);
	if (*line == '/' || ft_strstr(line, "..") != NULL
					|| (fd = open(line, O_RDONLY)) < 0 || !verify(fd))
	{
		write_s8(stream, 0);
		write_string(stream, "No such file or directory", 25);
		return (1);
	}
	write_s8(stream, 1);
	while ((readres = read(fd, block, 4096)) > 0)
	{
		if (!write_string(stream, block, readres))
			return (0);
	}
	if (!write_string(stream, "", 0))
		return (0);
	return (1);
}
