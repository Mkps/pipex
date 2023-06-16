#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

char    *ft_getenv(char **env, const char *str);
void    argc_error(int error_code);
void    error_exit(int exit_code);
int     open_fd(int mode, char *filename);



#endif
