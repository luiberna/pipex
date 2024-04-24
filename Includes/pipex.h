/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luiberna <luiberna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:58:43 by luiberna          #+#    #+#             */
/*   Updated: 2024/04/23 01:34:42 by luiberna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"

// utils
void	free_list(char **list);
char	*get_path(char *cmd, char **envp);
void	execute_w_path(char **cmd, char **envp);
void	execute(char *argv, char **envp);

// pipex
void	child(char **argv, char **envp, int *fd);
void	child2(char **argv, char **envp, int *fd);
void	pipex(char **argv, char **envp);

#endif