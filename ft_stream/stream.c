/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/30 13:10:34 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/31 16:25:22 by roblabla         ###   ########.fr       */
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
// Garantees at least one byte can be read, unless stream->size == 0.
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

	ptr = (int8_t*)nbr;
	int res =	read_s8(stream, ptr + 0)
			&&	read_s8(stream, ptr + 1)
			&&	read_s8(stream, ptr + 2)
			&&	read_s8(stream, ptr + 3);
	if (res)
		*nbr = (int32_t)ntohl(*nbr);
	return (res);
}

char			*ft_strnjoinfree(char *a, char *b, size_t blen)
{
	char	*c;
	char	*d;

	d = ft_strsub(b, 0, blen); // Screw optimization
	c = ft_strjoin(a, d);
	free(a);
	free(d);
	return (c);
}

int				read_string(t_stream *stream, char **str)
{
	int		strsize;
	int		striter;
	int		oldstriter;

	if (!read_s32be(stream, &strsize))
		return (0);
	striter = FT_MIN(stream->size - stream->cursor, (unsigned)strsize);
	oldstriter = striter;
	*str = ft_strsub(stream->buf, stream->cursor, striter);
	stream->cursor += striter;
	while (striter < strsize && (fill_buf(stream), stream->size) > 0)
	{
		striter += FT_MIN(stream->size - stream->cursor, (unsigned)strsize - striter);
		*str = ft_strnjoinfree(*str, stream->buf + stream->cursor,
				striter - oldstriter);
		stream->cursor += striter - oldstriter;
		oldstriter = striter;
	}
	return (striter == strsize);
}

// TODO : proper error handling v
int				write_s8(t_stream *stream, int8_t c)
{
	return (write(stream->fd, &c, 1) >= 0);
}

int				write_s32be(t_stream *stream, int32_t nbr)
{
	nbr = htonl(nbr);
	return (write(stream->fd, &nbr, sizeof(nbr)) >= 0);
}

int				write_string(t_stream *stream, char *s)
{
	size_t	nbr;

	if (s == NULL)
		s = "";
	return (write_s32be(stream, (nbr = ft_strlen(s))) &&
			write(stream->fd, s, nbr) >= 0);
}
