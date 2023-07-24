/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:47 by alx               #+#    #+#             */
/*   Updated: 2023/07/24 16:49:51 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex_bonus.h"

void	argc_error(int error_code)
{
	ft_putendl_fd("pipex: error: not enough arguments.", 2);
	if (error_code == 0)
		ft_putendl_fd("Standard use is ./pipex infile cmd cmd outfile", 2);
	if (error_code == 1)
	{
		ft_putstr_fd("Use with here_doc is ", 2);
		ft_putendl_fd("./pipex here_doc LIMITER cmd cmd1 file", 2);
	}
	exit(1);
}

//Use perror to display the errno and exit with the specified code.
void	error_exit(int exit_code)
{
	perror("pipex: an error occured.");
	exit(exit_code);
}

/** Get the env variable from envp specified by str. */
char	*ft_getenv(char **env, const char *str)
{
	int		i;
	char	*tmp;

	tmp = 0;
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(env[i], str, ft_strlen(str)))
			tmp = env[i] + ft_strlen(str) + 1;
		i++;
	}
	return (tmp);
}

int	open_fd(int mode, char *filename)
{
	int	fd;

	if (mode == 0)
		fd = open(filename, O_RDONLY, 0664);
	if (mode == 1)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd == -1)
	{
		ft_putstr_fd("pipex: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (fd);
	}
	return (fd);
}
