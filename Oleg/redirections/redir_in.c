// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <string.h>

//     // argv[1] eto "<"
//     // argv[2] v dannom sluchae eto "file dlja chtenija"

// // < — оператор перенаправления ввода. Используется для считывания ввода команды из файла.
// void msg_error(const char *msg)
// {
//     perror(msg);
//     exit(EXIT_FAILURE);
// }

// void redirect_input(const char *input_file)
// {
//     int infile = open(input_file, O_RDONLY); 
//     if (infile < 0)
//         msg_error("");

//     if (dup2(infile, STDIN_FILENO) < 0)
//         msg_error("");

//     close(infile);
// }

// int main(int argc, char *argv[])
// {
//     if (argc != 3)
//     {
//         msg_error("syntax error near unexpected token `newline");
//         exit(EXIT_FAILURE);
//     }

//     const char *input_file = argv[2];
//     redirect_input(input_file);

//     char buffer[1024];
//     ssize_t read_len;
//     while ((read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
//     {
//         buffer[read_len] = '\0';
//         printf("%s", buffer);
//     }

//     return 0;
// }

// //  ./a.out "<" text.txt

// /*
// Если вы хотите передать содержимое файла "input.txt" в команду grep для поиска строки "pattern":
// grep pattern < input.txt

// Если вы хотите передать содержимое файла "data.csv" в команду sort для сортировки данных:
// sort < data.csv

// Если у вас есть программа "my_program", которая читает ввод с клавиатуры, и вы хотите передать содержимое файла "input.txt" в качестве ввода:
// ./my_program < input.txt

// Если вы хотите передать содержимое файла "words.txt" в команду wc для подсчета числа строк, слов и символов:
// wc < words.txt

// Если у вас есть скрипт "my_script.sh", который ожидает ввод от пользователя, и вы хотите передать содержимое файла "input.txt" в качестве ввода в этот скрипт:
// ./my_script.sh < input.txt

// */


// // void msg_error(const char *msg)
// // {
// //     perror(msg);
// //     exit(EXIT_FAILURE);
// // }

// // void redirect_input(const char *filename)
// // {
// //     int infile = open(filename, O_RDONLY);
// //     if (infile < 0)
// //         msg_error("");

// //     if (dup2(infile, STDIN_FILENO) < 0)
// //         msg_error("");

// //     close(infile);
// // }

// // int main(int argc, char *argv[])
// // {
// //     if (argc != 3)
// //     {
// //         msg_error("syntax error near unexpected token `newline");
// //         exit(EXIT_FAILURE);
// //     }

// //     if (ft_strcmp(argv[1], "<") != 0)
// //     {
// //         msg_error("it's not redirection symbol < "); // po umu nado budet udalit ves etot if
// //         exit(EXIT_FAILURE);
// //     }

// //     const char *input_file = argv[2];
// //     redirect_input(input_file);

// //     char buffer[1024];
// //     ssize_t read_len;
// //     while ((read_len = read(STDIN_FILENO, buffer, sizeof(buffer) - 1)) > 0)
// //     {
// //         buffer[read_len] = '\0';
// //         printf("%s", buffer);
// //     }

// //     return 0;
// // }

// // int ft_strcmp(char *s1, char *s2) //libft
// // {
// //     while(*s1 && *s2)
// //     {
// //         if(*s1 != *s2)
// //             return(*s1 - *s2);
// //         s1++;
// //         s2++;
// //     }
// //     return(*s1 - *s2);
// // }