/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:25:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/16 15:48:46 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipes.h"
#include "types/commands.h"
#include "here_doc.h"
#include <fcntl.h>
#include <libft.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char	**environ;

static int	process_command(t_command command, int *fd, char **envp)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		execve(command.file, command.args, envp);
		free_command(command);
		return (EXIT_FAILURE);
	}
	wait(&pid);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	free_command(command);
	return (EXIT_SUCCESS);
}

static int	process(int argc, char **argv, int **fd, int use_here_doc)
{
	int			i;
	t_command	command;

	if (use_here_doc)
	{
		if (here_doc(argv) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
		dup2(open(argv[1], O_RDONLY), STDIN_FILENO);
	i = 0;
	while (i < argc - 3 - use_here_doc)
	{
		command = parse_command(argv[i + 2 + use_here_doc]);
		if (!command.file)
		{
			free_command(command);
			return (EXIT_FAILURE);
		}
		if (process_command(command, fd[i], environ) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

static void	copy_to(int from, int to)
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

int	main(int argc, char **argv)
{
	int	use_here_doc;
	int	**fd;
	int	backup[2];
	int	process_output;
	int	output;

	use_here_doc = ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0;
	if (argc - 3 - use_here_doc < 1)
		return (EXIT_FAILURE);
	fd = setup_pipes(argc - 3 - use_here_doc);
	if (!fd)
		return (EXIT_FAILURE);
	backup[0] = dup(STDIN_FILENO);
	backup[1] = dup(STDOUT_FILENO);
	process_output = process(argc, argv, fd, use_here_doc);
	if (process_output == EXIT_SUCCESS)
	{
		output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		copy_to(fd[argc - 4 - use_here_doc][0], output);
		close(output);
	}
	dup2(backup[0], STDIN_FILENO);
	dup2(backup[1], STDOUT_FILENO);
	close_pipes(fd);
	return (process_output);
}
