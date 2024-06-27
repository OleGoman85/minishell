#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MINI_H
# define MINI_H

    typedef struct s_builtin
    {
        char        **all_env;
        char        *variable;
        int         envp_size;
        char        **new_envp;
    }   t_builtin;


#endif