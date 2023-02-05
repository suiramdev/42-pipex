/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:25:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/05 19:55:07 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/commands.h"
#include <fcntl.h>
#include <libft.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void	close_pipes(int **fd)
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

static int	**setup_pipes(int count)
{
	int	**fd;
	int	i;

	fd = malloc(sizeof(int *) * (count) + 1);
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

int	main(int argc, char **argv, char **envp)
{
	int **fd;
	int i;
	pid_t pid;
	t_command cmd;
	char buffer[400];

	if (argc < 4)
		return (EXIT_FAILURE);
	fd = setup_pipes(argc - 2);
	if (!fd)
		return (EXIT_FAILURE);
	dup2(open(argv[1], O_RDONLY), STDIN_FILENO);
	i = 0;
	while (i < argc - 3)
	{
		pid = fork();
		if (pid < 0)
			return (EXIT_FAILURE);
		if (pid == 0)
		{
			dup2(fd[i][1], STDOUT_FILENO);
			cmd = parse_command(argv[i + 2]);
			execve(cmd.file, cmd.args, envp);
			return (EXIT_FAILURE);
		}
		wait(&pid);
		close(fd[i][1]);
		dup2(fd[i][0], STDIN_FILENO);
		i++;
	}
	while (read(fd[i - 1][0], buffer, 400) > 0)
		printf("%s", buffer);
	close_pipes(fd);
	return (EXIT_SUCCESS);
}