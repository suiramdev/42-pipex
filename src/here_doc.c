/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 18:17:36 by mnouchet          #+#    #+#             */
/*   Updated: 2023/02/16 15:49:01 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

int	here_doc(char **argv)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) < 0)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
		here_doc_loop(argv[2], fd);
	wait(&pid);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	return (EXIT_SUCCESS);
}
