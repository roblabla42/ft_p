/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 19:17:36 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 22:40:48 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "protocol.h"
#include "stream.h"

int		handle_pwd(t_stream *stream, char *line, char **cmd)
{
	char	*res;

	(void)line;
	(void)cmd;
	write_s8(stream, PWD);
	if (!read_string(stream, &res, NULL))
		return (0);
	ft_putendl("SUCCESS");
	ft_putendl(res);
	return (1);
}
