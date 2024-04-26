/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 01:27:39 by luiberna          #+#    #+#             */
/*   Updated: 2024/04/26 14:49:07 by luiberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_list(char **list)
{
	int	i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	*join_path;
	char	**paths;

	i = 0;
	while (envp[0] && ft_strnstr(envp[i], "PATH=", 5) == 0)
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

void	execute_w_path(char **cmd, char **envp)
{
	if (execve(cmd[0], cmd, envp) == -1)
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: Invalid command\n\e[0m", 33));
	}
}

void	execute(char *argv, char **envp)
{
	int		i;
	char	*path;
	char	**cmd;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!cmd[0])
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: No command\n\e[0m", 28));
	}
	if (cmd[0][0] == '/')
		execute_w_path(cmd, envp);
	path = get_path(cmd[0], envp);
	if (path == NULL)
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: Invalid command\n\e[0m", 33));
	}
	if (execve(path, cmd, envp) == -1)
	{
		free_list(cmd);
		exit(write(2, "\033[31mError: Invalid command\n\e[0m", 33));
	}
}
