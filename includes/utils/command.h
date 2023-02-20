/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:08:37 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/20 17:38:31 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

typedef struct s_command
{
	char	*file;
	char	**args;
}	t_command;

t_command	parse_command(char *str);
void		free_command(t_command command);
int			execute_command(t_command command, int *pipe, char **envp);

#endif
