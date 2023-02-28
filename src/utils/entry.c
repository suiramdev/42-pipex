/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 16:46:24 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/28 16:30:14 by suiramdev        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

static int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i])
	{
		if (!s1[i])
			return (0);
		i++;
	}
	return (s1[i] - s2[i]);
}

static void	here_doc_loop(char *delim, int fd[2])
{
	char	*line;

	close(fd[0]);
	line = ft_gnl(STDIN_FILENO);
	while (line)
	{
		line[ft_strlen(line) - 1] = '\0';
		if (ft_strcmp(line, delim) == 0)
			break ;
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = ft_gnl(STDIN_FILENO);
	}
	free(line);
	exit(0);
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
