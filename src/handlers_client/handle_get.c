/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:45:41 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/04 01:59:04 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <stdlib.h>
#include <libft.h>
#include <fcntl.h>
#include <unistd.h>
#include "protocol.h"
#include "stream.h"

static int	pipefile(t_stream *stream, char **cmd)
{
	char	*tmp;
	size_t	tmpsize;
	int		newfile;
	int		readres;

	tmp = cmd[1];
	while (ft_strchr(tmp, '/') != NULL)
		tmp = ft_strchr(tmp, '/') + 1;
	if ((newfile = open(tmp,
					O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0)
	{
		ft_putendl("ERROR: Error opening file");
		return (2);
	}
	while ((readres = read_string(stream, &tmp, &tmpsize)) && tmpsize != 0)
	{
		write(newfile, tmp, tmpsize);
		free(tmp);
	}
	if (!readres)
		return (0);
	close(newfile);
	return (1);
}

int			handle_get(t_stream *stream, char *line, char **cmd)
{
	int8_t	res;
	char	*resstr;
	int		readres;

	(void)line;
	if (cmd[1] == NULL)
		ft_putendl("ERROR: Usage : get <path>");
	else
	{
		write_s8(stream, GET);
		write_string(stream, cmd[1], ft_strlen(cmd[1]));
		if ((readres = read_s8(stream, &res)) && res)
		{
			if (!(res = pipefile(stream, cmd)))
				return (0);
			else if (res == 1)
				ft_putendl("SUCCESS");
		}
		else if (readres && read_string(stream, &resstr, NULL))
			(ft_putstr("ERROR: "), ft_putendl(resstr));
		else
			return (0);
	}
	return (1);
}
