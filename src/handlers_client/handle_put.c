/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_put.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 19:16:36 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 19:17:21 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <libft.h>
#include "stream.h"
#include "protocol.h"

int		handle_put(t_stream *stream, char *line, char **cmd)
{
	int		newfd;
	int		size;
	char	buf[BUFSIZE];

	(void)line;
	if (cmd[1] == NULL || (newfd = open(cmd[1], O_RDONLY)) < 0)
		ft_putendl("Usage : put <path>");
	else
	{
		write_s8(stream, PUT);
		write_string(stream, cmd[1], ft_strlen(cmd[1]));
		while ((size = read(newfd, buf, BUFSIZE)) > 0)
			write_string(stream, buf, size);
		write_string(stream, "", 0);
	}
	return (1);
}
