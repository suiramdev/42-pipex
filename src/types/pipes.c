/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:48:29 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 20:20:36 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/pipes.h"
#include <stdlib.h>

t_pipes	*pipes_new(size_t count)
{
	t_pipes	*pipes;

	if (count <= 0)
		return (NULL);
	pipes = malloc(sizeof(t_pipes));
	if (!pipes)
		return (NULL);
	pipes->fds = malloc(sizeof(int *) * count);
	if (!pipes->fds)
	{
		free(pipes);
		return (NULL);
	}
	pipes->count = 0;
	while (pipes->count < count)
	{
		pipes->fds[pipes->count] = malloc(sizeof(int) * 2);
		if (!pipes->fds[pipes->count] || pipe(pipes->fds[pipes->count]) < 0)
		{
			pipes_del(pipes);
			return (NULL);
		}
		pipes->count++;
	}
	return (pipes);
}

void	pipes_del(t_pipes *pipes)
{
	size_t	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < pipes->count)
	{
		close(pipes->fds[i][0]);
		close(pipes->fds[i][1]);
		free(pipes->fds[i]);
		i++;
	}
	free(pipes->fds);
	free(pipes);
}
