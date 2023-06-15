#include "pipex.h"
#include <string.h>
#include <sys/wait.h>

char    *get_cmd(char *cmd, char **env_p);
void    do_cmd(char *cmd, char **env_p);

void    argc_error(int error_code)
{
    if (error_code == 0)
        printf("pipex: error: not enough arguments.\nStandard use is ./pipex infile cmd cmd outfile\n");
    if (error_code == 1)
        printf("pipex: error: not enough arguments.\nUse with here_doc is ./pipex here_doc LIMITER cmd cmd1 file\n");
    exit(1);
}

void    error_exit(int exit_code)
{
    perror("pipex: an error occured.");
    exit(exit_code);
}

void    here_doc_input(char *limiter, int *fd)
{
    char    *str;

    close(fd[0]);

    while(1)
    {
        str = get_next_line(0);
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

void    here_doc_handler(char *limiter)
{
    int p_fd[2];
    pid_t   pid;
    int status;

    if(pipe(p_fd) == -1)
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
        waitpid(-1, &status, 0);
        if(status != 256)
            printf("pipex: warning: here-doc delimited by end-of-file (wanted `%s').\n", limiter);
        dup2(p_fd[0], 0);
        close(p_fd[0]);
    }
}
/** Get the env variable from envp specified by str. */
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
    exit(0);
}

void    do_pipe(char *cmd, char **env_p)
{
    pid_t   pid;
    int     pipe_fd[2];

    if(pipe(pipe_fd) == -1)
        error_exit(3);
    pid = fork();
    if (pid == -1)
        error_exit(44);
    if (pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        do_cmd(cmd, env_p);
        close(pipe_fd[1]);
    }
    else
    {
        close(pipe_fd[1]);
        dup2(pipe_fd[0],0);
        close(pipe_fd[0]);
    }
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
		error_exit(9);
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

    close(pipe_fd[0]);
    fd = open(argv[1], O_RDONLY, 0777);
    if (fd == -1)
        open_error(env_p);
    dup2(fd, 0);
    dup2(pipe_fd[1], 1);
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
    char    *env;
    char    **env_p;
    int     i;

    if (argc < 5)
        argc_error(0);
    if (!strcmp(argv[1], "here_doc"))
    {
        i = 3;
        if (argc < 6)
            argc_error(1);
        pipe_fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        here_doc_handler(argv[2]);
    }
    else
    {
        i = 2;
        pipe_fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        pipe_fd[0] = open(argv[1], O_RDONLY);
        dup2(pipe_fd[0], 0);
    }
    env = ft_getenv(envp, "PATH");
    env_p = ft_split(env, ':');
    while (i < argc - 2)
        do_pipe(argv[i++], env_p);
    dup2(pipe_fd[1], 1);
    do_cmd(argv[argc - 2], env_p);
    ft_free_tab(env_p);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    exit(0);
}
