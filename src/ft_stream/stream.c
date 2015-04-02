/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 19:23:36 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:44:39 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <unistd.h>
#include "stream.h"
#include <stdlib.h>
#include <arpa/inet.h>

void			create_stream(int fd, t_stream *stream)
{
	stream->fd = fd;
	stream->size = read(fd, stream->buf, 4096);
	stream->cursor = 0;
}

void			fill_buf(t_stream *stream)
{
	if (stream->size > 0 && (ssize_t)stream->cursor >= stream->size)
	{
		stream->size = read(stream->fd, stream->buf, 4096);
		stream->cursor = 0;
	}
}

int				read_s8(t_stream *stream, int8_t *c)
{
	fill_buf(stream);
	if (stream->size <= 0)
		return (0);
	else
	{
		*c = stream->buf[stream->cursor++];
		return (1);
	}
}

int				read_s32be(t_stream *stream, int32_t *nbr)
{
	int8_t	*ptr;
	int		res;

	ptr = (int8_t*)nbr;
	res = read_s8(stream, ptr + 0)
			&& read_s8(stream, ptr + 1)
			&& read_s8(stream, ptr + 2)
			&& read_s8(stream, ptr + 3);
	if (res)
		*nbr = (int32_t)ntohl(*nbr);
	return (res);
}

int				read_string(t_stream *stream, char **str, size_t *size)
{
	size_t	strsize;
	size_t	striter;
	int		oldstriter;
	char	*tmp;

	if (!read_s32be(stream, (int32_t*)&strsize))
		return (0);
	*str = malloc(sizeof(char) * (strsize + 1));
	(*str)[strsize] = '\0';
	tmp = *str;
	striter = 0;
	oldstriter = 0;
	while (striter < strsize && stream->size > 0)
	{
		striter += FT_MIN(stream->size - stream->cursor, strsize - striter);
		ft_memmove(tmp, stream->buf + stream->cursor, striter - oldstriter);
		tmp += striter - oldstriter;
		stream->cursor += striter - oldstriter;
		oldstriter = striter;
		if (striter < strsize)
			fill_buf(stream);
	}
	if (size != NULL)
		*size = (size_t)strsize;
	return (striter == strsize);
}
