/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:25:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/09 15:09:51 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include "types/commands.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
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
			if (cmd.file)
				execve(cmd.file, cmd.args, envp);
			free_command(cmd);
			return (EXIT_FAILURE);
		}
		wait(&pid);
		close(fd[i][1]);
		dup2(fd[i][0], STDIN_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}

static void	write_to(int from, int to)
{
	int		bytes;
	char	buffer[400];

	bytes = read(from, buffer, 400);
	while (bytes > 0)
	{
		write(to, buffer, bytes);
		bytes = read(from, buffer, 400);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	backup[2];
	int	**fd;
	int	output;

	if (argc < 4)
		return (EXIT_FAILURE);
	fd = setup_pipes(argc - 2);
	if (!fd)
		return (EXIT_FAILURE);
	backup[0] = dup(STDIN_FILENO);
	backup[1] = dup(STDOUT_FILENO);
	if (process(argv, envp, fd) == EXIT_SUCCESS)
	{
		output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write_to(fd[argc - 4][0], output);
		close(output);
	}
	dup2(backup[0], STDIN_FILENO);
	dup2(backup[1], STDOUT_FILENO);
	close_pipes(fd);
	return (EXIT_SUCCESS);
}
