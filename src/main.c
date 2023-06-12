#include "pipex.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    int   fd[2];
    pid_t pid;

    if (argc < 5)
        printf("Hello world!\n");
    if (pipe(fd) == -1)
        return (-1);
    pid = fork();
    (void)argv;
    if (pid == -1)
        return (0);
    return (0);
}
