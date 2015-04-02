/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 23:18:13 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:18:34 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <unistd.h>
#include <libft.h>

int		handle_pwd(t_stream *stream, t_state *state)
{
	char	cwd[MAXPATHLEN];

	(void)state;
	if (getcwd(cwd, MAXPATHLEN) == NULL)
		return (0);
	write_string(stream, cwd, ft_strlen(cwd));
	return (1);
}
