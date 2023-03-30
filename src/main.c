/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:29:44 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 20:39:55 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/command.h"
#include "types/pipes.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>

static void	child_process(size_t i, t_command *command,
		t_pipes *pipes, int entries[2])
{
	if (i > 0)
		dup2(pipes->fds[i - 1][0], STDIN_FILENO);
	else
		dup2(entries[0], STDIN_FILENO);
	if (command->next)
		dup2(pipes->fds[i][1], STDOUT_FILENO);
	else
		dup2(entries[1], STDOUT_FILENO);
	pipes_del(pipes);
	execve(command->path, command->args, command->env);
	close(entries[0]);
	if (entries[1] != -1)
		close(entries[1]);
	command_del(command);
	exit(EXIT_FAILURE);
}

/// @brief Wait for all the processes in the list of commands.
/// @param commands The list of commands.
/// @return void
void	wait_processes(t_command *commands)
{
	t_command	*command;

	command = commands;
	while (command)
	{
		waitpid(command->pid, NULL, 0);
		command = command->next;
	}
}

/// @brief Run all the processes in the list of commands.
/// @param entries The entries and exits of the pipeline.
/// @param commands The list of commands.
/// @param pipes The list of pipes.
/// @return void
static void	run_processes(int entries[2], t_command *commands, t_pipes *pipes)
{
	t_command	*command;
	size_t		i;

	command = commands;
	i = 0;
	while (command)
	{
		command->pid = fork();
		if (command->pid == 0)
			child_process(i, command, pipes, entries);
		if (i > 0)
			close(pipes->fds[i - 1][0]);
		if (command->next)
			close(pipes->fds[i][1]);
		command = command->next;
		i++;
	}
	wait_processes(commands);
}

int	main(int argc, char **argv, char **envp)
{
	int			entries[2];
	int			i;
	t_command	*commands;
	t_pipes		*pipes;

	if (argc < 5)
		return (EXIT_FAILURE);
	entries[0] = open(argv[1], O_RDONLY);
	i = 2 + (entries[0] == -1);
	if (entries[0] == -1)
		entries[0] = open("/dev/null", O_RDONLY);
	entries[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	commands = NULL;
	while (i < argc - 1 && argv[i])
		push_command(&commands, command_new(argv[i++], envp));
	pipes = pipes_new(count_commands(commands));
	if (pipes)
		run_processes(entries, commands, pipes);
	command_del(commands);
	close(entries[0]);
	if (entries[1] != -1)
		close(entries[1]);
	pipes_del(pipes);
	return (EXIT_SUCCESS);
}
