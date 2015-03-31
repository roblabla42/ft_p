/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roblabla </var/spool/mail/roblabla>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/31 08:51:59 by roblabla          #+#    #+#             */
/*   Updated: 2015/03/31 12:57:39 by roblabla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
#include <sys/param.h>
typedef struct	s_state
{
	int			id;
	int			running;
	char		rootdir[MAXPATHLEN];
}				t_state;
#endif
