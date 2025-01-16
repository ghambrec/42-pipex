/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:27:17 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/16 11:37:38 by ghambrec         ###   ########.fr       */
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
		free_split(args);
		ft_putendl_fd("Could not find the program", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	if (execve(path, args, env) == -1)
	{
		ft_putstr_fd("Error executing command ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		perror(" ");
		free_split(args);
		exit(EXIT_FAILURE);
	}
}

void	child(char *infile, char *cmd, int *pipefd, char **env)
{
	int	infile_fd;

	infile_fd = open(infile, O_RDONLY);
	if (infile_fd == -1)
	{
		perror("Error opening infile");
		exit(EXIT_FAILURE);
	}
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_cmd(cmd, env);
}

void	parent(char *outfile, char *cmd, int *pipefd, char **env)
{
	int	outfile_fd;

	outfile_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile_fd == -1)
	{
		ft_printf("Error opening outfile");
		exit(EXIT_FAILURE);
	}
	// dup2(outfile_fd, STDOUT_FILENO); //for testing print in the terminal not in the file
	close(outfile_fd);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_cmd(cmd, env);
}

int	main(int argc, char **argv, char **env)
{
	int	pipefd[2];
	int	error_check;
	int	pid1;

	if (argc != 5)
	{
		ft_putendl_fd("Bad arguments! Expection: ./pipex file1 cmd1 cmd2 file2", STDERR_FILENO);
		return (EXIT_FAILURE);	
	}
	error_check = pipe(pipefd);
	if (error_check == -1)
		return (perror("Error opening pipe"), EXIT_FAILURE);
	pid1 = fork();
	if (pid1 < 0)
		return (perror("Error opening child-process"), EXIT_FAILURE);
	if (pid1 == 0)
	{
		child(argv[1], argv[2], pipefd, env);
	}
	parent(argv[4], argv[3], pipefd, env);
	waitpid(pid1, NULL, 0);
}
