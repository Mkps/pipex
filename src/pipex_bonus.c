/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/02 10:35:14 by alx               #+#    #+#             */
/*   Updated: 2023/08/02 11:44:50 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	free_pipex(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->nb_cmd - 1)
	{
		free(p->p_arr[i]);
		i++;
	}
	free(p->p_arr);
	free(p->pid);
	free(p->status);
	free(p);
}

void	pipex_init(t_pipex *handler)
{
	handler->count = handler->nb_cmd;
	handler->pid = (pid_t *)malloc(sizeof(pid_t) * handler->nb_cmd);
	handler->p_arr = (int **)malloc(sizeof(int *) * handler->nb_cmd - 1);
	handler->status = (int *)malloc(sizeof(int) * 4);
	*handler->status = 0;
}
