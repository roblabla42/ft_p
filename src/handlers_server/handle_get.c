/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:17:41 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:18:00 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <fcntl.h>
#include <unistd.h>

int		handle_get(t_stream *stream, t_state *state)
{
	int		fd;
	char	*line;
	char	block[4096];
	ssize_t	readres;

	(void)state;
	if (!read_string(stream, &line, NULL))
		return (0);
	fd = open(line, O_RDONLY);
	while ((readres = read(fd, block, 4096)) > 0)
	{
		if (!write_string(stream, block, readres))
			return (0);
	}
	if (!write_string(stream, "", 0))
		return (0);
	return (1);
}
