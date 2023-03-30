/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 18:31:51 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 20:05:53 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include <unistd.h>

typedef struct s_command
{
	char				*path;
	char				**args;
	char				**env;
	pid_t				pid;
	struct s_command	*prev;
	struct s_command	*next;
}	t_command;

t_command	*command_new(char *line, char **env);
void		push_command(t_command **commands, t_command *command);
size_t		count_commands(t_command *commands);
void		command_del(t_command *command);

#endif
