/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 23:08:20 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/20 21:35:50 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/command.h"
#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/// @brief Contatenate the given path with the given file name.
/// @param path The path.
/// @param file The file name.
/// @return The concatenation of the path and the file name.
static char	*join_path(char *path, char *file)
{
	char	*slashed;
	char	*concat;

	slashed = ft_strjoin("/", file);
	concat = ft_strjoin(path, slashed);
	free(slashed);
	return (concat);
}

/// @brief This function retrieve the given file's path,
/// by looking to the current directory and the PATH env.
/// @param file The file name.
/// @return The file's path, or NULL if the file is not found.
static char	*resolve_file(char *file)
{
	char	*path;
	char	**env;
	size_t	i;

	if (access(file, X_OK) >= 0)
		return (file);
	env = ft_env("PATH");
	if (!env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		path = join_path(env[i], file);
		if (access(path, X_OK) >= 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
	free(file);
	return (path);
}

/// @brief Parse the given string into a command.
/// @param str The string to parse.
/// @return The parsed command.
t_command	parse_command(char *str)
{
	t_command	command;
	char		**args;

	args = ft_split(str, ' ');
	command.file = resolve_file(ft_strdup(args[0]));
	command.args = args;
	return (command);
}

/// @brief Free the given command.
/// @param command The command to free.
void	free_command(t_command command)
{
	int	i;

	free(command.file);
	i = 0;
	while (command.args[i])
	{
		free(command.args[i]);
		i++;
	}
	free(command.args);
}

/// @brief Execute the given command.
/// @param command The command to execute.
/// @param pipe The pipe to use, to store the command's output. 
/// @param envp The environment variables, used to execute the command.
/// @return boolean
int	execute_command(t_command command, int *pipe, char **envp)
{
	int	pid;

	if (!command.file)
	{
		free_command(command);
		return (dup2(open("/dev/null", O_RDONLY), STDIN_FILENO) < 0);
	}
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe[0]);
		if (dup2(pipe[1], STDOUT_FILENO) < 0)
			return (0);
		execve(command.file, command.args, envp);
		free_command(command);
		return (1);
	}
	wait(&pid);
	close(pipe[1]);
	free_command(command);
	return (dup2(pipe[0], STDIN_FILENO) < 0);
}
