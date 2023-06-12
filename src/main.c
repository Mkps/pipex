#include "pipex.h"

// void    child_process(int *pipe_fd, char **argv, char **path)
// {
//     // int fd;

//     // fd = open(av[1], 1);
//     // dup2(fd, 0);
// }

int main(int argc, char **argv, char **env_p)
{
    int     pipe_fd[2];
    pid_t   pid;
    char    **cmd;
    char    *env;
    char    **path;
    char    *exec;
    char    *path_part;
    int     i;

    if (argc != 5)
        (void)env_p;
    env = getenv("PATH");
    path = ft_split(env, ':');
    cmd = ft_split(argv[1], ' ');
    i = -1;
    while (path[++i])
	{
		path_part = ft_strjoin(path[i], "/");
		exec = ft_strjoin(path_part, argv[1]);
		free(path_part);
        exec = ft_strtrim(exec, ">");
        printf("%s\n", exec);
		if (access(exec, F_OK | X_OK) == 0)
		{
            execve(exec, cmd, env_p);
		}
		free(exec);
	}
    if (pipe(pipe_fd) == -1)
        return (-1);
    pid = fork();
    (void)argv;
    if (pid == -1)
        return (1);
    if (!pid)
    //     // child_process();
    // else
    //     parent_process();
    return (0);
}
