/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stream.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/29 15:08:23 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/30 19:21:44 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STREAM_H
# define STREAM_H
# define BUFSIZE 4096
#include <stdint.h>

typedef struct	s_stream
{
	int			fd;
	char		buf[BUFSIZE];
	ssize_t		size;
	size_t		cursor;
}				t_stream;


void			create_stream(int fd, t_stream *stream);
int				read_s8(t_stream *stream, int8_t *c);
int				read_s32be(t_stream *stream, int32_t *c);
int				read_string(t_stream *stream, char **str);
int				read_line(t_stream *stream, char **str);
int				write_s8(t_stream *stream, int8_t c);
int				write_s32be(t_stream *stream, int32_t c);
int				write_string(t_stream *stream, char *s);
int				write_line(t_stream *stream, char *s);
#endif
