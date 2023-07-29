/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:50:00 by alx               #+#    #+#             */
/*   Updated: 2023/07/24 16:50:25 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
#include <sys/types.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>

typedef struct s_pipex {
	int	count;
	int	fd[2];
	int	*status;
	int	nb_cmd;
	pid_t	*pid;
	int	**p_arr;	
} t_pipex;

char	*ft_getenv(char **env, const char *str);
void	argc_error(int error_code);
void	error_exit(int exit_code);
int		open_fd(int mode, char *filename);
void	here_doc_handler(char *limiter);
void	exec_pipe(t_pipex *handler, char **cmd, char **envv);
void	exec_cmd(char *cmd, char **envv);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);
void	last_child(t_pipex *p, char *cmd, char **envv);
void	middle_child(t_pipex *p, char *cmd, char **envv);
void	first_child(t_pipex *p, char *cmd, char **envv);
void	parent_handler(t_pipex *p);

#endif
