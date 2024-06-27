#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


int ft_strcmp(char *s1, char *s2) //libft
{
    while(*s1 && *s2)
    {
        if(*s1 != *s2)
            return(*s1 - *s2);
        s1++;
        s2++;
    }
    return(*s1 - *s2);
}

void msg_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void redirect_output(const char *output_file, int append)
{
    int outfile_fd;
    int flags;
    flags = O_WRONLY | O_CREAT;
    if (append)
        flags = flags | O_APPEND;
    else
        flags = flags | O_TRUNC;

    outfile_fd = open(output_file, flags, 0644);
    if (outfile_fd < 0)
        msg_error("");

    if (dup2(outfile_fd, STDOUT_FILENO) < 0)
        msg_error("");

    close(outfile_fd);
}

int main(int argc, char *argv[])
{

    if (ft_strcmp(argv[2], ">") == 0)
        redirect_output(argv[3], 0);
    else if (ft_strcmp(argv[2], ">>") == 0)
        redirect_output(argv[3], 1);
    else
    {
        msg_error("proverka > ili >> nado udalit!!!");
        exit(EXIT_FAILURE);
    }

    int input_file_fd = open(argv[1], O_RDONLY);
    if (input_file_fd < 0)
        msg_error("Error opening input file");

    char buffer[1024];
    ssize_t read_len;
    while ((read_len = read(input_file_fd, buffer, sizeof(buffer))) > 0)
        write(STDOUT_FILENO, buffer, read_len);

    if (read_len < 0)
        msg_error("Error reading input file");

    close(input_file_fd);
    return 0;
}

/*
    >> — оператор добавления вывода. Используется для записи вывода команды в конец файла, не перезаписывая его содержимое.
    > — оператор перенаправления вывода. Используется для записи вывода команды в файл, создавая его, если он не существует, или перезаписывая его, если он существует.
*/


/* 
Если вы хотите записать содержимое файла "input.txt" в файл "output.txt" с помощью >:
cat input.txt > output.txt

Если вы хотите добавить содержимое файла "input.txt" в конец файла "output.txt" с помощью >>:
cat input.txt >> output.txt

Если вы хотите записать вывод команды ls -l в файл "file_list.txt" с помощью >:
ls -l > file_list.txt

Если вы хотите добавить вывод команды ls -l в конец файла "file_list.txt" с помощью >>:
ls -l >> file_list.txt

Если у вас есть исполняемая программа "my_program", и вы хотите записать ее вывод в файл "output.txt" с помощью >:
./my_program > output.txt

Если у вас есть исполняемая программа "my_program", и вы хотите добавить ее вывод в конец файла "output.txt" с помощью >>:
./my_program >> output.txt

*/



// int ft_strcmp(char *s1, char *s2); //libft

// void msg_error(const char *msg)
// {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// void redirect_output(const char *output_file, int append)
// {
//     int flags = O_WRONLY | O_CREAT;
//     if (append)
//         flags = flags | O_APPEND;
//     else
//         flags = flags | O_TRUNC;

//     int outfile = open(output_file, flags, 0644);
//     if (outfile < 0)
//         msg_error("");

//     if (dup2(outfile, STDOUT_FILENO) < 0)
//         msg_error("");

//     close(outfile);
// }

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         msg_error("syntax error near unexpected token `newline");
//         exit(EXIT_FAILURE);
//     }

//     if (ft_strcmp(argv[1], ">") == 0)
//         redirect_output(argv[2], 0);
//     if (ft_strcmp(argv[1], ">>") == 0)
//         redirect_output(argv[2], 1);

//     // test
//     printf("proverka vivoda.\n");

//     return 0;
// }

// int ft_strcmp(char *s1, char *s2) //libft
// {
//     while(*s1 && *s2)
//     {
//         if(*s1 != *s2)
//             return(*s1 - *s2);
//         s1++;
//         s2++;
//     }
//     return(*s1 - *s2);
// }
