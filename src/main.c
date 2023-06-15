#include "pipex.h"
#include <string.h>
#include <sys/wait.h>

char    *get_cmd(char *cmd, char **env_p);
void    exec_cmd(char *cmd, char **env_p);

/** Get the input */
void    here_doc_input(char *limiter, int *fd)
{
    char    *str;

    close(fd[0]);
    str = "str";
    while(str)
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


void    exec_pipe(char *cmd, char **env_p)
{
    pid_t   pid;
    int     pipe_fd[2];

    if(pipe(pipe_fd) == -1)
        error_exit(33);
    pid = fork();
    if (pid == -1)
        error_exit(44);
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
        dup2(pipe_fd[0],STDIN_FILENO);
        close(pipe_fd[0]);
    }
}

void    exec_cmd(char *cmd, char **env_p)
{
    char    **cmd_split;
    char    *cmd_p;
    char    **sq;

    cmd_split = ft_split(cmd, ' ');\
    if (cmd_split[1][0] == 39)
    {
        free(cmd_split[1]);
        sq = ft_split(cmd, 39);
        cmd_split[1] = sq[1];
        cmd_split[2] = NULL;
    }
    // cmd_split[1] = split_q[1];
    cmd_p = get_cmd(cmd_split[0], env_p);
    if (execve(cmd_p, cmd_split, env_p) == -1)
    {
        ft_free_tab(env_p);
        ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
		ft_putendl_fd(cmd_split[0], STDERR_FILENO);
        ft_free_tab(sq);
		ft_free_tab(cmd_split);
		exit(9);
    }
    ft_free_tab(cmd_split);
}

char    *get_cmd(char *cmd, char **env_p)
{
    int i;
    char    *cmd_dir;
    char    *cmd_tmp;
    i = -1;
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
    return (cmd);
}

char **get_path(char **envp)
{
    char **env_p;
    char *env;

    (void)envp;
    env = getenv("PATH");
    env_p = ft_split(env, ':');
    return(env_p);
}

int main(int argc, char **argv, char **envp)
{
    int     pipe_fd[2];
    char    **env_p;
    int     i;

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
        pipe_fd[0] = open_fd(STDIN_FILENO, argv[1]);
        pipe_fd[1] = open_fd(STDOUT_FILENO, argv[argc - 1]);
        dup2(pipe_fd[0], 0);
    }
    env_p = get_path(envp);
    while (i != argc - 2)
        exec_pipe(argv[i++], env_p);
    dup2(pipe_fd[1], STDOUT_FILENO);
    exec_cmd(argv[argc - 2], env_p);
    ft_free_tab(env_p);
    exit(0);
}
