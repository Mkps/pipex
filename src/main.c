#include "pipex.h"

char    *get_cmd(char *cmd, char **env_p);

void    do_cmd(char *cmd, char **env_p)
{
    char    **cmd_split;
    char    *cmd_p;

    cmd_split = ft_split(cmd, ' ');
    cmd_p = get_cmd(cmd_split[0], env_p);
    printf("cmd = %s\n", cmd_p);
    if (execve(cmd_p, cmd_split, env_p) == -1)
    {
        ft_putstr_fd("Error", 2);
        exit(0);
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
    dup2(fd, 0);
    // close(pipe_fd[1]);
    // close(pipe_fd[0]);
    (void)pipe_fd;
    do_cmd(argv[2], env_p);
}

int main(int argc, char **argv)
{
    int     pipe_fd[2];
    // pid_t   pid;
    char    *env;
    char    **env_p;

    if (argc != 3)
        return (0);
    env = getenv("PATH");
    env_p = ft_split(env, ':');

    // if (pipe(pipe_fd) == -1)
    //     return (-1);
    // pid = fork();
    // (void)argv;
    // if (pid == -1)
    //     return (1);
    // if (!pid)
    child_process(pipe_fd, argv, env_p);
    // else
    //     parent_process();
    return (0);
}
