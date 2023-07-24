/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:50:07 by alx               #+#    #+#             */
/*   Updated: 2023/07/24 16:50:08 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <errno.h>

char	*ft_getenv(char **env, const char *str);
void	argc_error(int error_code);
void	error_exit(int exit_code);
int		open_fd(int mode, char *filename);
void	here_doc_handler(char *limiter);
void	exec_pipe(char *cmd, char **envv);
void	exec_cmd(char *cmd, char **envv);
char	*get_cmd(char *cmd, char **env_p);
char	**get_path(char **envv);

#endif
