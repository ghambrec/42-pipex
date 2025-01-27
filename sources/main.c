/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:27:17 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/27 13:17:04 by ghambrec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		ft_putstr_fd("Bad arguments! Expection: ", STDERR_FILENO);
		ft_putendl_fd("./pipex file1 cmd1 cmd2 file2", STDERR_FILENO);
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
	return (EXIT_SUCCESS);
}
