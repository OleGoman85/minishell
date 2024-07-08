/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:31:10 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/04 12:25:52 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
/* perror() */

# include <unistd.h>
/* STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO,
	access(): F_OK, X_OK, fork(), execve(), pipe(), dup2()
*/

# include <stdlib.h>
/* malloc(), calloc(), free() */

# include <fcntl.h>
/* O_RDONLY (read only), O_RDWR (read and write), O_CREAT(create),
O_TRUNC (will overwrite if the file already exists), open(), close()
*/

# include <sys/types.h>
/* pid_t, size_t */

typedef struct s_data
{
    pid_t   *pid;        // Массив PID для дочерних процессов
    char    **av;        // Аргументы командной строки
    char    **env;       // Массив переменных окружения (envp)
    char    **path;      // Пути к командам, например, "/bin/ls" или "/usr/bin/grep"
    char    ***cmd_opt;  // Опции команды, например, "ls", "-l"
    int     **pipes_fd;  // Двумерный массив файловых дескрипторов для pipe (читает/пишет)
    int     pipes_n;     // Количество pipe (равно количеству команд минус один)
    int     input_fd;    // Файловый дескриптор для входного файла (чтение)
    int     output_fd;   // Файловый дескриптор для выходного файла (запись)
    int     status;      // Статус завершения команд
    int     num_cmds;    // Количество команд
    int     envp_size;   // Размер массива переменных окружения
    char    *variable;   // Переменная для хранения аргументов export
    char    **new_envp;  // Массив для обновленных переменных окружения
    int     cmd_index;   // Индекс команды в массиве
    char    *limiter;    // Строка, содержащая limiter для heredoc (heredoc)
    int     here_doc;    // Флаг для определения, используется ли heredoc (heredoc)
    char    *symbol;     // Символ для определения типа редиректа(heredoc)
} t_data;


////////////////// minishel //////////

void create_pipes(t_data *data);
void close_pipes_fd(t_data *data);
void free_pipes(t_data *data);
void main_errors(int error_code);
void execute_command(t_data *data, char **cmd, int input_fd, int output_fd);
void exec_cmd(t_data *data, char **cmd);

// int		px_strcmp(const char *s1, const char *s2);
// int		px_strncmp(const char *s1, const char *s2, size_t n);
// void	processes_making(t_data *data);
// void	process(char *cmd, t_data *data);
// void	main_errors(int error);
// void	path_errors(int error);
// void	child_pid1(t_data *data);
// void	child_pid2(t_data *data);
// void	close_fd(t_data *data);
// void	px_bzero(void *s, size_t n);
// void	*px_calloc(size_t count, size_t size);
// char	*px_strjoin(char const *s1, char const *s2);
// char	*px_strdup(const char *s);
// char	*px_substr(char const *s, unsigned int start, size_t len);
// void	*px_memset(void *ptr, int value, size_t num);
// char	**px_split(char const *str, char c);
// size_t	px_strlen(const char *s);
// size_t	px_strlcpy(char *dst, const char *src, size_t n);

#endif

/*
	pid_t	pid1/pid2; process identifier after calling the fork()
	char	**av; This is a pointer to a pointer to character
	char	**env; environment variables passed to the program
	char	**path; used to store paths to directories
	char	**cmd_opt; used to store command options passed to the program.
	int		fd[2]; create a pipe between two processes,reading and writing
	int		file_in; file descriptor for the input file (read)
	int		file_out; file descriptor for the output file (write)
	int		status; status code of the process
*/

