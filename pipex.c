/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 12:11:26 by luiberna          #+#    #+#             */
/*   Updated: 2024/04/18 20:38:27 by luiberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_list(char **list)
{
	int i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

char	*get_path(char *cmd, char **envp)
{
	int i;
	char *path;
	char *join_path;
	char **paths;

	i = 0;
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		join_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(join_path, cmd);
		free(join_path);
		if (access(path, F_OK) == 0)
			return (free_list(paths), path);
		free(path);
 		i++;
	}
	return (free_list(paths), NULL);
}
void execute(char *argv, char **envp)
{
	int i;
	char *path;
	char **cmd;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!cmd[0])
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: No command\n\e[0m", 29));
	}
	path = get_path(cmd[0], envp);
	if (path == NULL)
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: Invalid command\n\e[0m", 34));
	}
	if (execve(path, cmd, envp) == -1)
	{
		ft_printf("\033[31mError: Invalid command\n\e[0m");
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
	close(fd[1]);
	close(file);
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
	close(fd[0]);
	close(fileout);
	execute(argv[3], envp);
}

void pipex(char **argv, char **envp)
{
	int fd[2];
	pid_t pid;
	
	if (pipe(fd) == -1)
		{
			ft_printf("\033[31mError: Pipe not working, call Mario\n\e[0m", 2);
			exit (1);
		}
		pid = fork();
		if (pid == -1)
			exit(ft_printf("\033[31mError: Invalid PID\n\e[0m", 2));
		if (pid == 0)
			child(argv, envp, fd);
		else
			parent(argv, envp, fd);
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
