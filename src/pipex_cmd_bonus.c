/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmd_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:33 by alx               #+#    #+#             */
/*   Updated: 2023/08/02 11:06:02 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	exec_pipe(t_pipex *p, char **cmd, char **envv)
{
	int	index;

	index = 1;
	first_child(p, *cmd, envv);
	while (index < p->nb_cmd - 1)
	{
		middle_child(index, p, *(cmd + index), envv);
		index++;
	}
	last_child(index, p, *(cmd + index), envv);
	parent_handler(p);
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
		ft_putstr_fd("\'", 2);
		ft_putstr_fd(cmd_split[0], 2);
		ft_putendl_fd("\' command not found", 2);
		ft_free_tab(cmd_split);
	}
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
