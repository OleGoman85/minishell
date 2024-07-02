#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MINI_H
# define MINI_H

typedef struct s_data
{
    pid_t   *pid;        // Массив PID для дочерних процессов // eto dlja pipex nado budet
    char    **av;        // Аргументы командной строки
    char    **env;       // Массив переменных окружения (envp)
    char    **path;      // Пути к командам, например, "/bin/ls" или "/usr/bin/grep"
    char    ***cmd_opt;   // Опции команды, например, "ls", "-l"
    int     **pipes_fd;  // Двумерный массив файловых дескрипторов для pipe (читает/пишет)
    int     pipes_n;     // Количество pipe (равно количеству команд минус один)
    int     input_fd;    // Файловый дескриптор для входного файла (чтение)
    int     output_fd;   // Файловый дескриптор для выходного файла (запись)
    int     status;      // Статус завершения команд
    int     num_cmds;    // Количество команд
    int     envp_size;   // Размер массива переменных окружения
    char    *variable;   // Переменная для хранения аргументов export
    char    **new_envp;  // Массив для обновленных переменных окружения
    int     cmd_index    //индекс команды в массиве
} t_data;


#endif