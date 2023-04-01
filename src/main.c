/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:29:44 by mnouchet          #+#    #+#             */
/*   Updated: 2023/04/01 13:13:28 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entry.h"
#include "process.h"
#include "types/pipes.h"
#include "types/command.h"
#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int	main(int argc, char **argv, char **envp)
{
	int			entries[2];
	int			i;
	t_command	*commands;

	if (argc < 5)
		return (EXIT_FAILURE);
	i = 2 + entry((int *[2]){&entries[0], &entries[1]}, argc, argv);
	commands = NULL;
	while (i < argc - 1 && argv[i])
		push_command(&commands, command_new(argv[i++], envp));
	run_processes(entries, commands);
	command_del(commands);
	close(entries[0]);
	unlink(".here_doc");
	if (entries[1] != -1)
		close(entries[1]);
	return (EXIT_SUCCESS);
}
