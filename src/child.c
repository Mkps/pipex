/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 05:51:41 by alx               #+#    #+#             */
/*   Updated: 2023/07/25 05:56:24 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	first_child(int *end, int *pipe_fd, char **argv, char **envv)
{
	dup2(pipe_fd[0], 0);
	dup2(end[1], 1);
	close(end[0]);
	exec_cmd(argv[2], envv);
}

void	last_child(int *end, int *pipe_fd, char **argv, char **envv)
{
	dup2(pipe_fd[1], 1);
	dup2(end[0], STDIN_FILENO);
	close(end[1]);
	exec_cmd(argv[3], envv);
}

void	parent_handler(int *end, pid_t *pid, int *status)
{
	close(end[0]);
	close(end[1]);
	waitpid(pid[0], status, 0);
	waitpid(pid[1], status, 0);
}