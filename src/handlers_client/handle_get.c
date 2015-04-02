/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:45:41 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 18:48:30 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdlib.h>
#include <libft.h>
#include <fcntl.h>
#include <unistd.h>
#include "protocol.h"
#include "stream.h"

int		handle_get(t_stream *stream, char *line, char **cmd)
{
	char	*tmp;
	size_t	tmpsize;
	int		newfile;

	(void)line;
	if (cmd[1] == NULL)
		ft_putendl("Usage : get <path>");
	else
	{
		write_s8(stream, GET);
		write_string(stream, cmd[1], ft_strlen(cmd[1]));
		if ((newfile = open(cmd[1],
						O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0)
			perror("newfile");
		while (read_string(stream, &tmp, &tmpsize) && tmpsize != 0)
		{
			write(newfile, tmp, tmpsize);
			free(tmp);
		}
		close(newfile);
	}
	return (1);
}
