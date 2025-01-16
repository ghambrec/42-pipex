/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:27:17 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/16 12:08:07 by ghambrec         ###   ########.fr       */
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

void	child(char *infile, char *cmd, int *pipe_fd, char **env)
{
	int	infile_fd;

	infile_fd = open(infile, O_RDONLY);
	if (infile_fd == -1)
	{
		perror("Error opening infile");
		exit(errno);
	}
	if (dup2(infile_fd, STDIN_FILENO) == -1)
	{
		perror("Error dup2 infile_fd to stdin");
		exit(errno);
	}
	close(infile_fd);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("Error dup2 pipe_fd to stdout");
		exit(errno);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec_cmd(cmd, env);
}

void	parent(char *outfile, char *cmd, int *pipe_fd, char **env)
{
	int	outfile_fd;

	outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile_fd == -1)
	{
		perror("Error opening outfile");
		exit(EXIT_FAILURE);
	}
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
	{
		perror("Error dup2 outfile_fd to stdout");
		exit(errno);
	}
	close(outfile_fd);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("Error dup2 pipe_fd to stdin");
		exit(errno);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exec_cmd(cmd, env);
}

int	main(int argc, char **argv, char **env)
{
	int	pipe_fd[2];
	int	error_check;
	int	pid1;

	if (argc != 5 || ft_strlen(argv[2]) == 0 || ft_strlen(argv[3]) == 0)
	{
		ft_putendl_fd("Bad arguments! Expection: ./pipex file1 cmd1 cmd2 file2", STDERR_FILENO);
		return (EXIT_FAILURE);	
	}
	error_check = pipe(pipe_fd);
	if (error_check == -1)
		return (perror("Error opening pipe"), errno);
	pid1 = fork();
	if (pid1 < 0)
		return (perror("Error opening child-process"), errno);
	if (pid1 == 0)
	{
		child(argv[1], argv[2], pipe_fd, env);
	}
	parent(argv[4], argv[3], pipe_fd, env);
	if (waitpid(pid1, NULL, 0) == -1)
		return (perror("Error waiting for child-process"), errno);
	return (0);
}
