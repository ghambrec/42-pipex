/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:44:46 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/16 12:47:28 by ghambrec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_split(char **split)
{
	char	**orig;

	orig = split;
	while (*split != NULL)
	{
		free(*split);
		split++;
	}
	free(orig);
}

static char	*get_path_base(char **env)
{
	while (*env != NULL)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			break ;
		env++;
	}
	if (*env == NULL)
	{
		ft_putendl_fd("Error finding PATH", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (*env + 5);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*half_path;
	char	*full_path;
	int		i;

	paths = ft_split(get_path_base(env), ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		half_path = ft_strjoin(paths[i], "/");
		if (!half_path)
			return (free_split(paths), NULL);
		full_path = ft_strjoin(half_path, cmd);
		free(half_path);
		if (!full_path)
			return (free_split(paths), NULL);
		if (access(full_path, F_OK | X_OK) == 0)
			return (free_split(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_split(paths), NULL);
}

void	exec_cmd(char *cmd, char **env)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = get_path(args[0], env);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(args[0], STDERR_FILENO);
		free_split(args);
		exit(EXIT_FAILURE);
	}
	if (execve(path, args, env) == -1)
	{
		ft_putstr_fd("Error executing command ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		perror(" ");
		free_split(args);
		exit(errno);
	}
}
