#include "pipex.h"

void    argc_error(int error_code)
{
    if (error_code == 0)
        printf("pipex: error: not enough arguments.\nStandard use is ./pipex infile cmd cmd outfile\n");
    if (error_code == 1)
        printf("pipex: error: not enough arguments.\nUse with here_doc is ./pipex here_doc LIMITER cmd cmd1 file\n");
    exit(1);
}

/** Use perror to display the errno of the error and exit with the specified code.*/
void    error_exit(int exit_code)
{
    perror("pipex: an error occured.");
    exit(exit_code);
}

/** Get the env variable from envp specified by str. */
char    *ft_getenv(char **env, const char *str)
{
    int     i;
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

int    open_fd(int mode, char *filename)
{
    int fd;

    fd = -1;
    if (mode == 0)
        fd = open(filename, O_RDONLY, 0777);
    if (mode == 1)
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd == -1)
    {
        perror("pipex: error opening file");
        exit(0);
    }
    return (fd);
}
