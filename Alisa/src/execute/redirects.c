#include "minishell.h"

/**
 * @brief Обрабатывает перенаправление ввода.
 *
 * Эта функция открывает файл для чтения, сохраняет текущий stdin, 
 * перенаправляет stdin на открытый файл, выполняет команду, восстанавливает 
 * первоначальный stdin и закрывает файл. Если файл не может быть открыт, 
 * возвращает код ошибки.
 *
 * @param redirect Структура перенаправления, содержащая информацию о файле и команде.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Код завершения выполнения команды или код ошибки.
 */
int	process_input_redirection(t_redir *redirect, t_shell *shell)
{
	int		fd;
	int		exit_code;
	int		stdin_temp;
	bool	success;

	fd = open(redirect->filename, O_RDONLY);
	success = (fd != -1);
	if (success)
	{
		stdin_temp = dup(STDIN_FILENO);
		success = (duplicate_fd(fd, STDIN_FILENO, shell) != -1);
		close_file(fd, shell);
	}
	if (success)
	{
		exit_code = run_cmd(redirect->command, OP_COMPLETE, shell);
		duplicate_fd(stdin_temp, STDIN_FILENO, shell);
		close_file(stdin_temp, shell);
	}
	else
		exit_code = error_msg_errno(redirect->filename, shell);
	return (exit_code);
}


/**
 * @brief Обрабатывает перенаправление вывода.
 *
 * Эта функция открывает файл для записи (с созданием или добавлением), 
 * сохраняет текущий stdout, перенаправляет stdout на открытый файл, выполняет 
 * команду, восстанавливает первоначальный stdout и закрывает файл. Если файл 
 * не может быть открыт, возвращает код ошибки.
 *
 * @param redirect Структура перенаправления, содержащая информацию о файле и команде.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Код завершения выполнения команды или код ошибки.
 */
int	process_output_redirection(t_redir *redirect, t_shell *shell)
{
	int		fd;
	int		exit_code;
	int		stdout_temp;
	bool	success;

	if (redirect->redir_type == T_OUTPUT)
		fd = open(redirect->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	success = (fd != -1);
	if (success)
	{
		stdout_temp = dup(STDOUT_FILENO);
		success = (duplicate_fd(fd, STDOUT_FILENO, shell) != -1);
		close_file(fd, shell);
	}
	if (success)
	{
		exit_code = run_cmd(redirect->command, OP_COMPLETE, shell);
		duplicate_fd(stdout_temp, STDOUT_FILENO, shell);
		close_file(stdout_temp, shell);
	}
	else
		exit_code = error_msg_errno(redirect->filename, shell);
	return (exit_code);
}


/**
 * @brief Обрабатывает перенаправление ввода или вывода.
 *
 * Эта функция определяет тип перенаправления (ввод или вывод) и вызывает 
 * соответствующую функцию для обработки перенаправления.
 *
 * @param redirect Структура перенаправления, содержащая информацию о файле и команде.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Код завершения выполнения команды или код ошибки.
 */
int	process_redirection(t_redir *redirect, t_shell *shell)
{
	if (redirect->redir_type == T_INPUT || redirect->redir_type == T_HDOC)
		return (process_input_redirection(redirect, shell));
	return (process_output_redirection(redirect, shell));
}
