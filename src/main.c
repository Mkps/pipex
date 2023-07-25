/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alx <alx@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 16:49:00 by alx               #+#    #+#             */
/*   Updated: 2023/07/25 05:56:09 by alx              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

// Get path from the env variable. 
// If envv is (null) set the path to linux defaults
char	**get_path(char **envv)
{
	char	**env_p;
	char	*env;

	env = ft_getenv(envv, "PATH");
	if (env == NULL)
	{
		free(env);
		env = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	}
	env_p = ft_split(env, ':');
	return (env_p);
}

void	fd_heredoc(int argc, char **argv, int *i, int pipe_fd[2])
{
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
	{
		*i = 3;
		if (argc < 6)
			argc_error(1);
		pipe_fd[1] = open_fd(STDOUT_FILENO, argv[argc - 1]);
		here_doc_handler(argv[2]);
	}
}

void	fd_handler(int argc, char **argv, int *i, int pipe_fd[2])
{
	int	tmp_fd;

	*i = 2;
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
		fd_heredoc(argc, argv, i, pipe_fd);
	else
	{
		pipe_fd[1] = open_fd(STDOUT_FILENO, argv[argc - 1]);
		pipe_fd[0] = open_fd(STDIN_FILENO, argv[1]);
		if (pipe_fd[0] == -1)
		{
			tmp_fd = open_fd(STDOUT_FILENO, "tmp.txt");
			pipe_fd[0] = open_fd(STDIN_FILENO, "tmp.txt");
			dup2(tmp_fd, 0);
		}
		if (pipe_fd[1] == -1)
			exit(1);
		dup2(pipe_fd[0], 0);
		unlink("tmp.txt");
	}
}

int	main(int argc, char **argv, char **envv)
{
	int		i;
	int		end[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	int		status;

	if (argc != 5)
		argc_error(0);
	fd_handler(argc, argv, &i, pipe_fd);
	pipe(end);
	pid[0] = fork();
	if (!pid[0])
		first_child(end, pipe_fd, argv, envv);
	else
	{
		pid[1] = fork();
		if (!pid[1])
			last_child(end, pipe_fd, argv, envv);
		else
			parent_handler(end, pid, &status);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	exit(WEXITSTATUS(status));
}
