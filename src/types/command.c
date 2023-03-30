/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:31:39 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 20:26:15 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/command.h"
#include "libft.h"
#include "utils/path.h"

/// @brief Create a new command.
/// @param line The line to parse.
/// @param env The environment variables.
/// @return The new command.
t_command	*command_new(char *line, char **env)
{
	t_command	*command;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->env = env;
	command->args = ft_split(line, ' ');
	command->path = resolve_path(command->args[0], env);
	if (!command->path)
	{
		command_del(command);
		return (NULL);
	}
	command->next = NULL;
	command->prev = NULL;
	return (command);
}

/// @brief Push a command to the end of the list.
/// @param commands The list of commands.
/// @param command The command to push.
/// @return void
void	push_command(t_command **commands, t_command *command)
{
	t_command	*tmp;

	if (!command)
		return ;
	if (!*commands)
	{
		*commands = command;
		return ;
	}
	tmp = *commands;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = command;
	command->prev = tmp;
}

size_t	count_commands(t_command *commands)
{
	size_t	i;

	i = 0;
	while (commands)
	{
		i++;
		commands = commands->next;
	}
	return (i);
}

/// @brief Delete a command.
/// @param command The command to delete.
/// @return void
void	command_del(t_command *command)
{
	t_command	*tmp;
	size_t		i;

	while (command)
	{
		tmp = command->next;
		i = 0;
		while (command->args[i])
			free(command->args[i++]);
		free(command->args);
		free(command->path);
		free(command);
		command = tmp;
	}
}
