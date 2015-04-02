/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:21:01 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:23:51 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "protocol.h"
#include <libft.h>

static void	send_success(t_stream *stream, char *msg)
{
	if (!write_s8(stream, ACK))
		ft_putendl("ERR");
	if (!write_string(stream, msg, ft_strlen(msg)))
		ft_putendl("ERR");
}

int			handle_quit(t_stream *stream, t_state *state)
{
	send_success(stream, "Bye");
	state->running = 0;
	return (1);
}
