/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_unsigned.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:46:01 by roblabla          #+#    #+#             */
/*   Updated: 2015/04/02 23:52:26 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stream.h"
#include <arpa/inet.h>

int				read_u8(t_stream *stream, uint8_t *c)
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

int				read_u32be(t_stream *stream, uint32_t *nbr)
{
	uint8_t	*ptr;
	int		res;

	ptr = (uint8_t*)nbr;
	res = read_u8(stream, ptr + 0)
			&& read_u8(stream, ptr + 1)
			&& read_u8(stream, ptr + 2)
			&& read_u8(stream, ptr + 3);
	if (res)
		*nbr = ntohl(*nbr);
	return (res);
}



