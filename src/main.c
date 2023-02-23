/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 19:25:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/21 16:02:08 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/pipes.h"
#include "utils/entry.h"
#include "utils/command.h"
#include <libft.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static	int	process_entry(int use_here_doc, char **argv)
{
	if (use_here_doc)
	{
		if (entry_here(argv[2]) < 0)
			return (0);
	}
	else
	{
		if (!entry_file(argv[1]))
			return (0);
	}
	return (1);
}

static int	process_commands(int count, char **args, char **envp, int **pipes)
{
	int			i;
	t_command	command;

	i = 0;
	while (i < count)
	{
		if (args[i][0] == '\0')
			continue ;
		command = parse_command(args[i]);
		if (!execute_command(command, pipes[i], envp))
			return (0);
		i++;
	}
	return (1);
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

static int	process(int argc, char **argv, char **envp, int **pipes)
{
	int	use_here_doc;
	int	output;

	use_here_doc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	if (!process_entry(use_here_doc, argv))
		return (EXIT_FAILURE);
	if (!process_commands(argc - 3 - use_here_doc,
			argv + 2 + use_here_doc, envp, pipes))
		return (EXIT_FAILURE);
	output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output < 0)
		return (EXIT_FAILURE);
	copy_to(pipes[argc - 4 - use_here_doc][0], output);
	close(output);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv, char **envp)
{
	int	use_here_doc;
	int	backup[2];
	int	**pipes;
	int	output;

	use_here_doc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	if (argc - 3 - use_here_doc < 1)
		return (EXIT_FAILURE);
	backup[0] = dup(STDIN_FILENO);
	backup[1] = dup(STDOUT_FILENO);
	if (backup[0] < 0 || backup[1] < 0)
		return (EXIT_FAILURE);
	pipes = setup_pipes(argc - 3 - use_here_doc);
	if (!pipes)
		return (EXIT_FAILURE);
	output = process(argc, argv, envp, pipes);
	close_pipes(pipes);
	if (dup2(backup[0], STDIN_FILENO) < 0 || dup2(backup[1], STDOUT_FILENO) < 0)
		return (EXIT_FAILURE);
	return (output);
}
