/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:23:06 by mnouchet          #+#    #+#             */
/*   Updated: 2023/04/01 13:13:06 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/pipes.h"
#include "types/command.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
static void	wait_processes(t_command *commands)
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
void	run_processes(int entries[2], t_command *commands)
{
	t_pipes		*pipes;
	t_command	*command;
	size_t		i;

	pipes = pipes_new(count_commands(commands));
	if (pipes)
	{
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
	pipes_del(pipes);
}
