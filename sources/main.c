/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghambrec <ghambrec@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 13:27:17 by ghambrec          #+#    #+#             */
/*   Updated: 2025/01/09 15:52:44 by ghambrec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(void)
{
	int pid = fork();
	ft_printf("%i\n", pid);
	wait(NULL);
	
	return (EXIT_SUCCESS);
}

// #include <stdio.h>

// int main(int ac, char **av, char **env) {
//     // Kommandozeilenargumente ausgeben
//     printf("Anzahl der Argumente: %d\n", ac);
//     for (int i = 0; i < ac; i++) {
//         printf("Argument %d: %s\n", i, av[i]);
//     }

//     // Umgebungsvariablen ausgeben
//     printf("\nUmgebungsvariablen:\n");
//     for (int i = 0; env[i] != NULL; i++) {
//         printf("%s\n", env[i]);
//     }

//     return 0;
// }
