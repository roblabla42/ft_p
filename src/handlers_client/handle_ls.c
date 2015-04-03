/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ls.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:49:56 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 19:04:32 by rlambert         ###   ########.fr       */
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
	if (!read_until(stream, &lsres, '\0'))
		return (0);
	ft_putendl("SUCCESS");
	ft_putstr(lsres);
	return (1);
}
