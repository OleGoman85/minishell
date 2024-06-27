#ifndef MINI_HERE_DOC
# define MINI_HERE_DOC


# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include "./gnl/get_next_line.h"

typedef struct s_data {
    int here_doc;
    char *limiter;
    int infile;
    int outfile;
    char *symbol;
} t_data;

void    msg_error(const char *msg);
size_t  doc_strlen(const char *s);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
void    here_doc(t_data *data);
size_t	ft_strcspn(const char *s, const char *reject);
void    here_doc_tmp_fd(t_data *data);

#endif

