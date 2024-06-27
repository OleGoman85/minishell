
#include "mini_here_doc.h"


// //////// HERE DOC ///

// // bash-3.2$ ./a.out "<<" "a"
// // > sfgdfgfd
// // > fgdf
// // > jghj
// // > a
// // File .here_doc_tmp successfully created and written to.
// // Here document saved in .here_doc_tmp


void msg_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

size_t doc_strlen(const char *s)  //iz libft
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return i;
}

int ft_strncmp(const char *s1, const char *s2, size_t n) //iz libft
{
    size_t i = 0;
    while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    if (i == n)
        return 0;
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

size_t	ft_strcspn(const char *s, const char *reject) // iz libft
{
	size_t		count;
	const char	*r;

	count = 0;
	while (*s != '\0')
	{
		r = reject;
		while (*r != '\0')
		{
			if (*s == *r)
			{
				return (count);
			}
			r++;
		}
		s++;
		count++;
	}
	return (count);
}

void here_doc(t_data *data)
{
    int file;
    char *buf;

    file = open(".here_doc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (file < 0)
        msg_error("Error opening .here_doc_tmp");
    while (1)
    {
        write(1, "> ", 2);
        buf = get_next_line(0);
        if (!buf)
            msg_error("Error reading input");
        buf[ft_strcspn(buf, "\n")] = '\0';
        if (!ft_strncmp(data->limiter, buf, doc_strlen(data->limiter) + 1))
        {
            free(buf);
            break;
        }
        write(file, buf, doc_strlen(buf));
        write(file, "\n", 1);
        free(buf);
    }
    close(file);
    here_doc_tmp_fd(data);
}

void here_doc_tmp_fd(t_data *data)
{
    if (access(".here_doc_tmp", F_OK) == 0)
        data->infile = open(".here_doc_tmp", O_RDONLY);
    else
        msg_error("File .here_doc_tmp not found after creation attempt.");
    if (data->infile < 0)
    {
        unlink(".here_doc_tmp");
        msg_error("Error opening .here_doc_tmp");
    }
}

int main(int argc, char **argv)
{
    t_data data;

    if (argc < 3)
    {
        msg_error("syntax error near unexpected token `newline");
        return EXIT_FAILURE;
    }

    data.symbol = argv[1];
    data.limiter = argv[2];

    if (!ft_strncmp("<<", data.symbol, 3))
    {
        data.here_doc = 1;
        here_doc(&data);
    }
    return 0;
}

