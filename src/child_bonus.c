/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 05:51:41 by alx               #+#    #+#             */
/*   Updated: 2023/07/25 05:56:24 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	close_pipe(t_pipex *p, int cmd_index)
{
	int i;	

	i = 0;
	cmd_index = p->nb_cmd - p->count;
	while (i < p->nb_cmd - 1)
	{
		if (i  + 1 != cmd_index)
			close(p->p_arr[i][1]);
		if (cmd_index != i)
			close(p->p_arr[i][0]);
		i++;
	}
}
void	first_child(t_pipex *p, char *cmd, char **envv)
{
	int	cmd_index;

	cmd_index = p->nb_cmd - p->count;
	p->count -= 1;
	p->pid[cmd_index] = fork();
	if (p->pid[cmd_index] == -1)
		printf("ERROR\n");
	if (p->pid[0] == 0)
	{
		// close_pipe(p, cmd_index);
		// close(p->p_arr[0][0]);
		// close(p->p_arr[0][1]);
		close(p->p_arr[0][0]);
		close(p->p_arr[1][1]);
		close(p->p_arr[1][0]);
		dup2(p->fd[0], 0);
		dup2(p->p_arr[0][1], 1);
		close(p->fd[0]);
		close(p->p_arr[0][1]);
		exec_cmd(cmd, envv);
	}
	else 
	{
		close(p->fd[0]);
		close(p->p_arr[0][1]);
	}
}

void	middle_child(t_pipex *p, char *cmd, char **envv)
{
	int	cmd_index;

	cmd_index = p->nb_cmd - p->count;
	p->count -= 1;
	p->pid[cmd_index] = fork();
	if (p->pid[cmd_index] == -1)
		printf("ERROR\n");
	if (p->pid[cmd_index] == 0)
	{
		close(p->p_arr[0][1]);
		close(p->p_arr[1][0]);
		dup2(p->p_arr[0][0], STDIN_FILENO);
		dup2(p->p_arr[1][1], STDOUT_FILENO);
		close(p->p_arr[0][0]);
		close(p->p_arr[1][1]);
		exec_cmd(cmd, envv);
	}
	else
	{
		close(p->p_arr[0][0]);
		close(p->p_arr[1][1]);
	}
}
void	last_child(t_pipex *p, char *cmd, char **envv)
{
	int	cmd_index;

	cmd_index = p->nb_cmd - p->count;
	p->count -= 1;
	p->pid[cmd_index] = fork();
	if (p->pid[cmd_index] == -1)
		printf("ERROR\n");
	if (p->pid[cmd_index] == 0)
	{
		close(p->p_arr[0][0]);
		close(p->p_arr[0][1]);
		close(p->p_arr[1][1]);
		dup2(p->p_arr[1][0], 0);
		dup2(p->fd[1], 1);
		close(p->p_arr[1][0]);
		close(p->fd[1]);
		exec_cmd(cmd, envv);
	}
	else 
	{
		close(p->p_arr[1][0]);
		close(p->fd[1]);
	}
}

void	parent_handler(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->nb_cmd )
	{
		if (i != p->nb_cmd - 1)
			waitpid(p->pid[i], NULL, 0);
		else
			waitpid(p->pid[i], &p->status, 0);
		i++;
	}
}
