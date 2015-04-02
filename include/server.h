/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 19:50:32 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/02 23:26:42 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# include <sys/param.h>
# include "stream.h"

typedef struct	s_state
{
	int			id;
	int			running;
	char		rootdir[MAXPATHLEN];
}				t_state;

typedef int		(*t_handler)(t_stream *, t_state *);
void			send_failure(t_stream *stream, char *msg);
int				handle_cd(t_stream *stream, t_state *state);
int				handle_put(t_stream *stream, t_state *state);
int				handle_get(t_stream *stream, t_state *state);
int				handle_ls(t_stream *stream, t_state *state);
int				handle_pwd(t_stream *stream, t_state *state);
int				handle_quit(t_stream *stream, t_state *state);
int				handle_fail(t_stream *stream, t_state *state);
void			handle_cmd(int cmd, t_stream *stream, t_state *state);
#endif
