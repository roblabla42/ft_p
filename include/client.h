/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlambert <rlambert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/04/02 18:18:07 by rlambert          #+#    #+#             */
/*   Updated: 2015/04/03 22:41:51 by rlambert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

int		handle_cd(t_stream *stream, char *line, char **cmd);
int		handle_get(t_stream *stream, char *line, char **cmd);
int		handle_ls(t_stream *stream, char *line, char **cmd);
int		handle_put(t_stream *stream, char *line, char **cmd);
int		handle_pwd(t_stream *stream, char *line, char **cmd);
int		handle_quit(t_stream *stream, char *line, char **cmd, int *running);

#endif
