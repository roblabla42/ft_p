/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:22:47 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 21:20:00 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "stream.h"
#include "protocol.h"

static int	read_result(t_stream *stream)
{
	int8_t	cmd;
	char	*reason;

	if (!read_s8(stream, &cmd))
		return (0);
	if (cmd == ACK || cmd == ERR)
	{
		ft_putstr(cmd == ACK ? "SUCCESS: " : "ERROR: ");
		if (!read_string(stream, &reason, NULL))
			return (0);
		else
			ft_putendl(reason);
	}
	else
	{
		ft_putendl("PROTOCOL ERROR");
		return (0);
	}
	return (1);
}

int			handle_quit(t_stream *stream, char *line, char **cmd)
{
	(void)line;
	(void)cmd;
	write_s8(stream, QUIT);
	read_result(stream);
	return (0);
}
