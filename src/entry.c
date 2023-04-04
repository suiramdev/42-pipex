/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:24:32 by mnouchet          #+#    #+#             */
/*   Updated: 2023/04/01 13:17:45 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entry.h"
#include <libft.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/// @brief Create a file descriptor for a here_doc
/// @param delimiter The delimiter of the here_doc
/// @return The file descriptor of the here_doc
static int	here_doc(char *delimiter)
{
	char	*line;
	char	*trimmed;
	int		fd;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (fd);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = ft_gnl(STDIN_FILENO);
		trimmed = ft_strtrim(line, "\n");
		if (ft_strncmp(trimmed, delimiter, ft_strlen(line) + 1) == 0)
		{
			free(line);
			free(trimmed);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
		free(trimmed);
	}
	close(fd);
	fd = open(".here_doc", O_RDONLY);
	return (fd);
}

/// @brief Entry point of the program
/// @param entries[0] Input file descriptor
/// @param entries[1] Output file descriptor
/// @param argc Number of arguments
/// @param argv arguments
/// @return 1 if the input file descriptor is a here_doc, or is /dev/null
/// @return 0 otherwise
int	entry(int *entries[2], int argc, char **argv)
{
	*(entries[1]) = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]) + 8) == 0)
	{
		*(entries[0]) = here_doc(argv[2]);
		return (1);
	}
	else
	{
		*(entries[0]) = open(argv[1], O_RDONLY);
		if (*(entries[0]) == -1)
		{
			*(entries[0]) = open("/dev/null", O_RDONLY);
			return (1);
		}
	}
	return (0);
}
