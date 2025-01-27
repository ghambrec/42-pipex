/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:29:25 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/27 13:16:29 by ghambrec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../sources/myLibft/include/my_libft.h"
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>

char	*get_path(char *cmd, char **env);
void	exec_cmd(char *cmd, char **env);

#endif