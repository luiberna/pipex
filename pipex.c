/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:11:26 by luiberna          #+#    #+#             */
/*   Updated: 2024/04/23 01:28:20 by luiberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child(char **argv, char **envp, int *fd)
{
	int	file;

	file = open(argv[1], O_RDONLY, 777);
	if (file == -1)
	{
		ft_printf("\033[31mError: Invalid file\n\e[0m", 2);
		exit(1);
	}
	dup2(fd[1], STDOUT_FILENO);
	dup2(file, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(file);
	execute(argv[2], envp);
}

void	child2(char **argv, char **envp, int *fd)
{
	int	fileout;

	fileout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
	{
		ft_printf("\033[31mError: Invalid file\n\e[0m", 2);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(fileout);
	execute(argv[3], envp);
}

void	pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid2;

	if (pipe(fd) == -1)
	{
		ft_printf("\033[31mError: Pipe not working, call Mario\n\e[0m", 2);
		exit(1);
	}
	pid = fork();
	if (pid == -1)
		exit(ft_printf("\033[31mError: Invalid PID\n\e[0m", 2));
	if (pid == 0)
		child(argv, envp, fd);
	pid2 = fork();
	if (pid2 == -1)
		exit(ft_printf("\033[31mError: Invalid PID\n\e[0m", 2));
	if (pid2 == 0)
		child2(argv, envp, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		return (ft_printf("\033[31mError: Not enough arguments\n\e[0m", 2), 0);
	if (!envp[0])
		return (ft_printf("\033[31mYou removed the env FY!\n\e[0m"));
	else
		pipex(argv, envp);
	return (0);
}
