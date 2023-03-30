/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:47:43 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 19:58:55 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

# include <unistd.h>

typedef struct s_pipes
{
	int			**fds;
	size_t		count;
}	t_pipes;

t_pipes		*pipes_new(size_t count);
void		pipes_del(t_pipes *pipes);

#endif
