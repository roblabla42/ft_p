/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:49:56 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:43:11 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <unistd.h>
#include "stream.h"
#include "protocol.h"

int		handle_ls(t_stream *stream, char *line, char **cmd)
{
	char	*lsres;

	(void)cmd;
	write_s8(stream, LS);
	write_string(stream, line, ft_strlen(line));
	if (!read_until(stream, &lsres, '\0')) // TODO : While loop, print as they come
		return (0);
	write(1, lsres, ft_strlen(lsres));
	return (1);
}
