/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/05 20:07:40 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/20 17:21:05 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

/// @brief Close all pipes
/// @param fd Array of pipes
void	close_pipes(int **fd)
{
	int	i;

	i = 0;
	while (fd[i])
	{
		close(fd[i][0]);
		close(fd[i][1]);
		free(fd[i]);
		i++;
	}
	free(fd);
}

/// @brief Setup pipes for a given amount
/// @param count Number of pipes to create
/// @return Array of pipes
int	**setup_pipes(int count)
{
	int	**fd;
	int	i;

	fd = malloc(sizeof(int *) * (count + 1));
	if (!fd)
		return (NULL);
	i = 0;
	while (i < count)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i] || pipe(fd[i]) < 0)
		{
			close_pipes(fd);
			return (NULL);
		}
		i++;
	}
	fd[i] = NULL;
	return (fd);
}
