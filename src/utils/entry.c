/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:46:24 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/20 21:35:16 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

static void	here_doc_loop(char *delim, int fd[2])
{
	char	*line;

	close(fd[0]);
	line = ft_gnl(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, delim, ft_strlen(delim)) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = ft_gnl(STDIN_FILENO);
	}
	free(line);
	exit(1);
}

/// @brief reproduce the behavior of the here_doc operator
/// @param delim the delimiter
/// @return the file descriptor read or -1 if an error occured
int	entry_here(char *delim)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		here_doc_loop(delim, fd);
	wait(&pid);
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) < 0)
		return (-1);
	return (STDIN_FILENO);
}

/// @brief Set the STDIN_FILENO to the file descriptor,
/// or to /dev/null if the file doesn't exist
/// @param file the file to open
/// @return boolean
int	entry_file(char *file)
{
	if (dup2(open(file, O_RDONLY), STDIN_FILENO) < 0)
	{
		if (dup2(open("/dev/null", O_RDONLY), STDIN_FILENO) < 0)
			return (0);
	}
	return (1);
}
