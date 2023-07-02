#include "pipex.h"

char	*get_cmd(char *cmd, char **env_p);
void	exec_cmd(char *cmd, char **env_p);

/** Get the input */
void	here_doc_input(char *limiter, int *fd)
{
	char	*str;

	close(fd[0]);
	str = "str";
	while (str)
	{
		str = get_next_line(0);
		if (str == NULL)
		{
			close(fd[1]);
			exit(2);
		}
		if (!ft_strncmp(str, limiter, ft_strlen(limiter)))
		{
			free(str);
			close(fd[1]);
			exit(1);
		}
		ft_putstr_fd(str, fd[1]);
		free(str);
	}
	close(fd[1]);
	free(str);
}

void	here_doc_handler(char *limiter)
{
	int		p_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(p_fd) == -1)
		error_exit(5);
	pid = fork();
	if (pid == -1)
		error_exit(6);
	status = 0;
	if (!pid)
		here_doc_input(limiter, p_fd);
	else
	{
		close(p_fd[1]);
		waitpid(pid, &status, 0);
		if (status != 256)
			printf("pipex: warning: here-doc end /w EOF(wanted `%s').\n",
				limiter);
		dup2(p_fd[0], 0);
		close(p_fd[0]);
	}
}

void	exec_pipe(char *cmd, char **env_p)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		error_exit(3);
	pid = fork();
	if (pid == -1)
		error_exit(4);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		exec_cmd(cmd, env_p);
		close(pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}

void	exec_cmd(char *cmd, char **env_p)
{
	char	**cmd_split;
	char	*cmd_p;
	char	**sq;
	char	sep;

	cmd_split = ft_split(cmd, ' ');
	sep = 0;
	if (cmd_split[1])
	{
		if (cmd_split[1][0] == 34 || cmd_split[1][0] == 39)
		{
			sep = cmd_split[1][0];
			free(cmd_split[1]);
			sq = ft_split(cmd, sep);
			cmd_split[1] = sq[1];
			cmd_split[2] = NULL;
		}
	}
	cmd_p = get_cmd(cmd_split[0], env_p);
	if (!cmd_p || execve(cmd_p, cmd_split, env_p) == -1)
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

char	**get_path(char **envp)
{
	char	**env_p;
	char	*env;

	env = ft_getenv(envp, "PATH");
	if (env == NULL)
	{
		free(env);
		env = "/:/usr/bin/";
	}
	env_p = ft_split(env, ':');
	return (env_p);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	int		tmp_fd;
	char	**env_p;
	int		i;
	int		pid;
	int		status;

	if (argc < 5)
		argc_error(0);
	i = 2;
	if (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")))
	{
		i++;
		if (argc < 6)
			argc_error(1);
		pipe_fd[1] = open_fd(STDOUT_FILENO, argv[argc - 1]);
		here_doc_handler(argv[2]);
	}
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
			exit(0);
		dup2(pipe_fd[0], 0);
		unlink("tmp.txt");
	}
	env_p = get_path(envp);
	while (i != argc - 2)
	{
		exec_pipe(argv[i], env_p);
		i++;
	}
	dup2(pipe_fd[1], STDOUT_FILENO);
	pid = fork();
	if (!pid)
		exec_cmd(argv[argc - 2], env_p);
	else
	{
		waitpid(pid, &status, 0);
		ft_free_tab(env_p);
		exit(WEXITSTATUS(status));
	}
}
