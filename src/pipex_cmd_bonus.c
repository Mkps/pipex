/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:33 by alx               #+#    #+#             */
/*   Updated: 2023/07/24 16:49:34 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	exec_pipe(int **end, t_pipex *handler, char **cmd, char **envv)
{
	int	cmd_index;
	int	count;
				
	cmd_index = handler->nb_cmd - handler->count;
	// printf("cmd_index %i\n", cmd_index);
	handler->count -= 1;
	// printf("count %i\n", handler->count);
	count = handler->count;
	handler->pid_array[cmd_index] = fork();
	if (handler->pid_array[cmd_index] == -1)
		error_exit(4);
	if (handler->pid_array[cmd_index] == 0)
	{
		if (count > 0)
		{
			exec_pipe(end, handler, (cmd + 1), envv);
		}
		if (cmd_index > 0)
			dup2(end[cmd_index - 1][0], STDIN_FILENO);
		if (count == 0)
			dup2(handler->fd[1], STDOUT_FILENO);
		else
			dup2(end[cmd_index][1], STDOUT_FILENO);
		if (cmd_index == 0)
			close(end[0][0]);
		else
			close(end[cmd_index][0]);
		close(end[cmd_index][1]);
		exec_cmd(*cmd, envv);
	}
	else
	{
		if (cmd_index == 0)
			close (handler->fd[0]);
		if (count == 0)
			close (handler->fd[1]);
		if (cmd_index)
			close(end[cmd_index - 1][0]);
		else
			close(end[cmd_index][0]);
		close(end[cmd_index][1]);
		waitpid(handler->pid_array[cmd_index], &handler->status, 0);
	}
}

static char	**escape_quote(char *cmd, char ***cmd_split, char *sep)
{
	char	**sq;

	sq = NULL;
	if (cmd_split[0][1])
	{
		if (cmd_split[0][1][0] == 34 || cmd_split[0][1][0] == 39)
		{
			*sep = cmd_split[0][1][0];
			free(cmd_split[0][1]);
			sq = ft_split(cmd, *sep);
			cmd_split[0][1] = sq[1];
			cmd_split[0][2] = NULL;
		}
	}
	return (sq);
}

void	exec_cmd(char *cmd, char **envv)
{
	char	**cmd_split;
	char	*cmd_p;
	char	**sq;
	char	sep;
	char	**env_p;

	env_p = get_path(envv);
	cmd_split = ft_split(cmd, ' ');
	sep = 0;
	cmd_p = get_cmd(cmd_split[0], env_p);
	sq = escape_quote(cmd, &cmd_split, &sep);
	cmd_p = get_cmd(cmd_split[0], env_p);
	if (!cmd_p || execve(cmd_p, cmd_split, envv) == -1)
	{
		ft_free_tab(env_p);
		if (sep)
			ft_free_tab(sq);
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(cmd_split[0], 2);
		ft_putendl_fd(": command not found", 2);
		ft_free_tab(cmd_split);
		exit(127);
	}
	ft_free_tab(env_p);
	ft_free_tab(cmd_split);
}

char	*get_cmd(char *cmd, char **env_p)
{
	int		i;
	char	*cmd_dir;
	char	*cmd_tmp;

	i = -1;
	if (access(cmd, F_OK | X_OK) == 0)
	{
		return (cmd);
	}
	while (env_p[++i])
	{
		cmd_dir = ft_strjoin(env_p[i], "/");
		cmd_tmp = ft_strjoin(cmd_dir, cmd);
		free(cmd_dir);
		if (access(cmd_tmp, F_OK | X_OK) == 0)
		{
			return (cmd_tmp);
		}
		free(cmd_tmp);
	}
	return (NULL);
}
