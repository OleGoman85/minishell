#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>




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

void redirect_output(t_data *data, const char *output_file, int append)
{
    int flags;
    if (!output_file)
        msg_error("Invalid output file");
    flags = O_WRONLY | O_CREAT;
    if (append)
        flags = flags | O_APPEND;
    else
        flags = flags | O_TRUNC;

    data->output_fd = open(output_file, flags, 0644);
    if (data->output_fd < 0)
        msg_error("");
    if (dup2(data->output_fd, STDOUT_FILENO) < 0)
        msg_error("");

    close(data->output_fd);
}

void redirect_input(t_data *data, const char *input_file)
{
    data->input_fd = open(input_file, O_RDONLY); 
    if (data->input_fd < 0)
        msg_error("");

    if (dup2(data->input_fd, STDIN_FILENO) < 0)
        msg_error("");

    close(data->input_fd);
}

int redirection(t_data *data)
{
    if (ft_strcmp(data->cmd_opt[data->cmd_index][0], ">") == 0)
        redirect_output(data, data->cmd_opt[data->cmd_index][1], 0);
    else if (ft_strcmp(data->cmd_opt[data->cmd_index][0], ">>") == 0)
        redirect_output(data, data->cmd_opt[data->cmd_index][1], 1);
    else if (ft_strcmp(data->cmd_opt[data->cmd_index][0], "<") == 0)
        redirect_input(data, data->cmd_opt[data->cmd_index][1]);
    else
    {
        msg_error("Invalid redirection operator");
        exit(EXIT_FAILURE);
    }
    data->input_fd = open(data->cmd_opt[data->cmd_index][1], O_RDONLY);
    if (data->input_fd < 0)
        msg_error("Error opening input file");
    char buffer[1024];
    ssize_t read_len;
    while ((read_len = read(data->input_fd, buffer, sizeof(buffer))) > 0)
        write(STDOUT_FILENO, buffer, read_len);
    if (read_len < 0)
        msg_error("Error reading input file");
    close(data->input_fd);
    return 0;
}