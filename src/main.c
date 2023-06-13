#include "pipex.h"

char    *get_cmd(char *cmd, char **env_p);

void    argc_error(void)
{
    perror("./pipex infile cmd cmd outfile\n");
    exit(1);
}

char    *ft_getenv(char **env, const char *str)
{
    int i;
    char    *tmp;

    tmp = 0;
    i = 0;
    while (env && env[i])
    {
        if (!ft_strncmp(env[i], str, ft_strlen(str)))
            tmp = env[i] + ft_strlen(str);
        i++;
    }
    return (tmp);
}
void    open_error(char **env_p)
{
    ft_free_tab(env_p);
    exit(1);
}
void    do_cmd(char *cmd, char **env_p)
{
    char    **cmd_split;
    char    *cmd_p;

    cmd_split = ft_split(cmd, ' ');
    cmd_p = get_cmd(cmd_split[0], env_p);
    if (execve(cmd_p, cmd_split, env_p) == -1)
    {
        ft_free_tab(env_p);
        ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd_split[0], 2);
		ft_free_tab(cmd_split);
		exit(1);
    }
}

char    *get_cmd(char *cmd, char **env_p)
{
    int i;
    char    *cmd_root;
    char    *cmd_tmp;
    i = -1;
    while (env_p[++i])
	{
		cmd_root = ft_strjoin(env_p[i], "/");
		cmd_tmp = ft_strjoin(cmd_root, cmd);
		free(cmd_root);
		if (access(cmd_tmp, F_OK | X_OK) == 0)
		{
            return (cmd_tmp);
		}
		free(cmd_tmp);
	}
    return (cmd);
}
void    child_process(int *pipe_fd, char **argv, char **env_p)
{
    int fd;

    fd = open(argv[1], O_RDONLY, 0777);
    if (fd == -1)
        open_error(env_p);
    dup2(fd, 0);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[0]);
    do_cmd(argv[2], env_p);
}

void    parent_process(int *pipe_fd, char **argv, char **env_p)
{
    int fd;

    fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        open_error(env_p);
    dup2(fd, 1);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[1]);
    do_cmd(argv[3], env_p);
}

int main(int argc, char **argv, char **envp)
{
    int     pipe_fd[2];
    pid_t   pid;
    char    *env;
    char    **env_p;

    if (argc != 5)
        argc_error();
    (void)envp;
    env = ft_getenv(envp, "PATH");
    env_p = ft_split(env, ':');
    if (pipe(pipe_fd) == -1)
        exit(1);
    pid = fork();
    (void)argv;
    if (pid == -1)
        exit(1);
    if (!pid)
        child_process(pipe_fd, argv, env_p);
    parent_process(pipe_fd, argv, env_p);
    ft_free_tab(env_p);
    exit(0);
}
