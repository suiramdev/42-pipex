/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:25:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/06 00:33:28 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include "types/commands.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int	process(char **argv, char **envp, int **fd)
{
	int			i;
	pid_t		pid;
	t_command	cmd;

	dup2(open(argv[1], O_RDONLY), STDIN_FILENO);
	i = 0;
	while (argv[i + 2])
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
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int		**fd;
	int		output;
	int		bytes;
	char	buffer[400];

	if (argc < 4)
		return (EXIT_FAILURE);
	fd = setup_pipes(argc - 2);
	if (!fd)
		return (EXIT_FAILURE);
	if (process(argv, envp, fd) == EXIT_FAILURE)
	{
		close_pipes(fd);
		return (EXIT_FAILURE);
	}
	output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	bytes = read(fd[argc - 4][0], buffer, 400);
	while (bytes > 0)
	{
		write(output, buffer, bytes);
		bytes = read(fd[argc - 4][0], buffer, 400);
	}
	close(output);
	close_pipes(fd);
	return (EXIT_SUCCESS);
}
