/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloubier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 17:20:47 by aloubier          #+#    #+#             */
/*   Updated: 2023/08/03 17:20:49 by aloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	close_pipe(t_pipex *p, int cmd_index)
{
	int	i;	

	i = 0;
	while (i < p->nb_cmd - 1)
	{
		if (cmd_index - 1 != i)
		{
			close(p->p_arr[i][0]);
		}
		if (cmd_index != i)
		{
			close(p->p_arr[i][1]);
		}
		i++;
	}
}

void	first_child(t_pipex *p, char *cmd, char **envv)
{
	p->count -= 1;
	p->pid[0] = fork();
	if (p->pid[0] == -1)
		ft_putstr_fd("pipex: error creating child process\n", 2);
	if (p->pid[0] == 0)
	{
		close_pipe(p, 0);
		if (p->here_doc == 0)
			dup2(p->fd[0], 0);
		dup2(p->p_arr[0][1], 1);
		if (p->here_doc == 0)
			close(p->fd[0]);
		close(p->fd[1]);
		close(p->p_arr[0][1]);
		exec_cmd(cmd, envv);
		free_pipex(p);
		exit(127);
	}
	else
	{
		if (p->here_doc == 0)
			close(p->fd[0]);
		close(p->p_arr[0][1]);
	}
}

void	middle_child(int cmd_index, t_pipex *p, char *cmd, char **envv)
{
	p->count -= 1;
	p->pid[cmd_index] = fork();
	if (p->pid[cmd_index] == -1)
		ft_putstr_fd("pipex: error creating child process\n", 2);
	if (p->pid[cmd_index] == 0)
	{
		close_pipe(p, cmd_index);
		close(p->fd[1]);
		dup2(p->p_arr[cmd_index - 1][0], STDIN_FILENO);
		dup2(p->p_arr[cmd_index][1], STDOUT_FILENO);
		close(p->p_arr[cmd_index - 1][0]);
		close(p->p_arr[cmd_index][1]);
		exec_cmd(cmd, envv);
		free_pipex(p);
		exit(127);
	}
	else
	{
		close(p->p_arr[cmd_index - 1][0]);
		close(p->p_arr[cmd_index][1]);
	}
}

void	last_child(int cmd_index, t_pipex *p, char *cmd, char **envv)
{
	p->count -= 1;
	p->pid[cmd_index] = fork();
	if (p->pid[cmd_index] == -1)
		ft_putstr_fd("pipex: error creating child process\n", 2);
	if (p->pid[cmd_index] == 0)
	{
		close_pipe(p, cmd_index);
		dup2(p->p_arr[cmd_index - 1][0], 0);
		dup2(p->fd[1], 1);
		close(p->p_arr[cmd_index - 1][0]);
		close(p->fd[1]);
		exec_cmd(cmd, envv);
		free_pipex(p);
		exit(127);
	}
	else
	{
		close(p->fd[1]);
		close(p->p_arr[cmd_index - 1][0]);
	}
}

void	parent_handler(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->nb_cmd)
	{
		if (i == p->nb_cmd - 1)
			waitpid(p->pid[i], p->status, 0);
		else
			waitpid(p->pid[i], NULL, 0);
		i++;
	}
}
