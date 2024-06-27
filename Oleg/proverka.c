#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// void msg_error(const char *msg) {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// void redirect_output(const char *output_file, int append) {
//     int flags = O_WRONLY | O_CREAT;
//     if (append)
//         flags |= O_APPEND;
//     else
//         flags |= O_TRUNC;

//     int outfile = open(output_file, flags, 0644);
//     if (outfile < 0)
//         msg_error("Error opening output file");

//     if (dup2(outfile, STDOUT_FILENO) < 0)
//         msg_error("Error redirecting stdout");

//     close(outfile);
// }

// int main(int argc, char *argv[])
// {

//     if (strcmp(argv[3], ">") == 0)
//         redirect_output(argv[4], 0);
//     else if (strcmp(argv[3], ">>") == 0)
//         redirect_output(argv[4], 1);
//     else {
//         fprintf(stderr, "Invalid redirection operator\n");
//         exit(EXIT_FAILURE);
//     }

//     FILE *input_file = fopen(argv[3], "r");
//     if (input_file == NULL) {
//         fprintf(stderr, "Error opening input file\n");
//         exit(EXIT_FAILURE);
//     }

//     char buffer[1024];
//     size_t read_len;
//     while ((read_len = fread(buffer, 1, sizeof(buffer), input_file)) > 0) {
//         fwrite(buffer, 1, read_len, stdout);
//     }

//     fclose(input_file);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void check(char *env_variable);

int main(int ac, char **av, char **envp) {
    int i = 0;
    while (envp[i] != NULL)
    {
        check(envp[i]); // Передаем каждую переменную окружения в функцию check
        i++;
    }
    return 0;
}

void check(char *env_variable) {
    int i = 0;
    while (env_variable[i] != '\0') { // Проверяем на нулевой символ для строки
        printf("%c", env_variable[i]);
        i++;
    }
    printf("\n"); // Печатаем новую строку после каждой переменной окружения
}
















#include <stdio.h>

void check(char *env_variable);

int main(int ac, char **av, char **envp) {
    int i = 0;
    while (envp[i] != NULL)
    {
        check(envp[i]); // Передаем каждую переменную окружения в функцию check
        i++;
    }
    return 0;
}

void check(char *env_variable) {
    int i = 0;
    while (env_variable[i] != '\0') { // Проверяем на нулевой символ для строки
        printf("%c", env_variable[i]);
        i++;
    }
    printf("\n"); // Печатаем новую строку после каждой переменной окружения
}
