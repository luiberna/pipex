/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:11:26 by luiberna          #+#    #+#             */
/*   Updated: 2024/04/16 18:03:14 by luiberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *argv, char **envp)
{
	int i;
	char *path;
	char *part_path;
	char **paths;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, argv);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free (path);
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	return (free(paths), NULL);
}
void execute(char *argv, char **envp)
{
	int i;
	char *path;
	char **cmd;

	i = 0;
	cmd = ft_split(argv, ' ');
	path = get_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		ft_printf("\033[31mError: invalid path\n\e[0m", 2);
		exit(1);
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_printf("\033[31mError: Invalid command\n\e[0m", 2);
		exit(1);
	}
}

void	child(char **argv, char **envp, int *fd)
{
	int file;

	file = open(argv[1], O_RDONLY, 777);
	if (file == -1)
	{
		ft_printf("\033[31mError: Invalid file\n\e[0m", 2);
		exit(1);
	}
	dup2(fd[1], STDOUT_FILENO);
	dup2(file, STDIN_FILENO);
	close(fd[0]);
	execute(argv[2], envp);
}

void	parent(char **argv, char **envp, int *fd)
{
	int		fileout;

	fileout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
	{
		ft_printf("\033[31mError: Invalid file\n\e[0m", 2);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(fileout, STDOUT_FILENO);
	close(fd[1]);
	execute(argv[3], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int fd[2];
	pid_t pid;

	if (argc != 5)
		return (ft_printf("\033[31mError: Not enough arguments\n\e[0m", 2), 0);
	else 
	{
		if (pipe(fd) == -1)
		{
			ft_printf("\033[31mError: Pipe not working, call Mario\n\e[0m", 2);
			return (0);
		}
		pid = fork();
		if (pid == -1)
			return (ft_printf("\033[31mError: Invalid PID\n\e[0m", 2), 0);
		if (pid == 0)
			child(argv, envp, fd);
		waitpid(pid, NULL, 0);
		parent(argv, envp, fd);
	}
	return (0);
}
