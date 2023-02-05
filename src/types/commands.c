/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:08:20 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/05 00:37:58 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types/commands.h"
#include <libft.h>
#include <unistd.h>

/**
 * This function concatenate the given path with the given file name.
 * Simple as that.
 *
 * @param path
 * @param file
 */
static char	*join_path(char *path, char *file)
{
	char	*slashed;
	char	*concat;

	slashed = ft_strjoin("/", file);
	concat = ft_strjoin(path, slashed);
	free(slashed);
	return (concat);
}

/**
 * This function retrieve the given file's path,
	by looking to the current directory and the PATH env.
 *
 * @param file
 */
static char	*resolve_file(char *file)
{
	char	*path;
	char	**env;
	size_t	i;

	if (access(file, X_OK) >= 0)
		return (file);
	env = ft_env("PATH");
	i = 0;
	while (env && env[i])
	{
		path = join_path(env[i], file);
		if (access(path, X_OK) >= 0)
			break ;
		free(path);
		i++;
	}
	while (env && *env)
	{
		free(*env);
		env++;
	}
	free(env);
	return (path);
}

t_command	parse_command(char *str)
{
	t_command	command;
	char		**args;

	args = ft_split(str, ' ');
	command.file = resolve_file(args[0]);
	command.args = args;
	return (command);
}
