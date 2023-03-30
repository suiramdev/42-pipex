/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/03 18:49:51 by mnouchet          #+#    #+#             */
/*   Updated: 2023/03/28 19:43:17 by mnouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/path.h"
#include <libft.h>
#include <unistd.h>

/// @brief Get the value of an environment variable.
/// @param env The environment variables.
/// @param key The key of the variable to get.
/// @return The value of the variable.
static char	**ft_getenv(char **env, char *key)
{
	size_t	i;

	while (env && *env)
	{
		i = 0;
		while (*(*env + i) == key[i])
			i++;
		if (*(*env + i) == '=')
			return (ft_split(*env + i + 1, ':'));
		env++;
	}
	return (NULL);
}

/// @brief Join a path and a file.
/// @param path The path.
/// @param file The file.
/// @return The joined path.
static char	*join_path(char *path, char *file)
{
	char	*new;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new = malloc(ft_strlen(path) + ft_strlen(file) + 2);
	if (!new)
		return (NULL);
	while (path[i])
	{
		new[i] = path[i];
		i++;
	}
	new[i++] = '/';
	while (file[j])
	{
		new[i + j] = file[j];
		j++;
	}
	new[i + j] = '\0';
	return (new);
}

/// @brief Resolve the path of a file.
/// @param file The file.
/// @param env The environment variables.
/// @return The resolved path.
char	*resolve_path(char *file, char **env)
{
	char	**paths;
	char	*output;
	size_t	i;

	if (!file || !env)
		return (NULL);
	if (access(file, X_OK) >= 0)
		return (ft_strdup(file));
	paths = ft_getenv(env, "PATH");
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		output = join_path(paths[i], file);
		if (access(output, X_OK) >= 0)
			break ;
		free(output);
		output = NULL;
		free(paths[i++]);
	}
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (output);
}
