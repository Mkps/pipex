#include "../includes/pipex_bonus.h"

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
	int		pipe_fd[2];
	int		i;
	int		pid;
	int		status;

	if (argc < 5)
		argc_error(0);
	fd_handler(argc, argv, &i, pipe_fd);
	while (i != argc - 2)
	{
		exec_pipe(argv[i], envv);
		i++;
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	pid = fork();
	if (!pid)
		exec_cmd(argv[argc - 2], envv);
	else
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pid, &status, 0);
		exit(WEXITSTATUS(status));
	}
}
