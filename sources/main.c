/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:27:17 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/15 20:24:45 by ghambrec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **split)
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

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*half_path;
	char	*full_path;
	int		i;

	while (*env != NULL)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			break;
		env++;
	}
	if (*env == NULL)
		return (NULL);
	paths = ft_split(*env + 5, ':');
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


int	main(int argc, char **argv, char **env)
{
	int	pipefd[2];
	int	error_check;
	int	pid1;
	int	pid2;

	// if (argc != 5)
	// 	return (EXIT_FAILURE);
	error_check = pipe(pipefd);
	if (error_check == -1)
		return (EXIT_FAILURE);

	pid1 = fork();
	if (pid1 < 0)
		return (EXIT_FAILURE);
	if (pid1 == 0)
	{
		// in the child process
		error_check = open(argv[1], O_RDONLY);
		if (error_check == -1)
		{
			ft_printf("error opening infile");
			return (EXIT_FAILURE);
		}
		dup2(error_check, STDIN_FILENO);
		close(error_check);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		char **args = ft_split(argv[2], ' ');
		char *path;
		path = get_path(args[0], env);
		if (execve(path, args, env) == -1)
		{
			ft_putstr_fd("error executing command: ", STDERR_FILENO);
			ft_putendl_fd(args[0], STDERR_FILENO);
			free_split(args);
			return (EXIT_FAILURE);
		}
	}
	pid2 = fork();
	if (pid2 < 0)
		return (EXIT_FAILURE);
	if (pid2 == 0)
	{
		error_check = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (error_check == -1)
		{
			ft_printf("error opening outfile");
			return (EXIT_FAILURE);
		}
		dup2(error_check, STDOUT_FILENO);
		close(error_check);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);

		char **args = ft_split(argv[3], ' ');
		char *path;
		path = get_path(args[0], env);
		if (execve(path, args, env) == -1)
		{
			ft_putstr_fd("error executing command: ", STDERR_FILENO);
			ft_putendl_fd(args[0], STDERR_FILENO);
			free_split(args);
			return (EXIT_FAILURE);
		}
	}


	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

}
