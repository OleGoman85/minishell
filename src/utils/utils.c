
#include "minishell.h"


/**
 * @brief Добавляет новую строку к массиву строк.
 *
 * Эта функция сначала подсчитывает количество строк в массиве,
 * затем создает новый массив с дополнительным местом для новой строки,
 * копирует строки из старого массива в новый и добавляет новую строку.
 *
 * @param args Указатель на массив строк.
 * @param str Новая строка для добавления.
 * @param shell Указатель на структуру оболочки для управления памятью.
 */
void	append_str(char ***args, char *str, t_shell *shell)
{
	int		original_count;
	int		i;
	char	**res_array;

	original_count = 0;
	i = 0;
	if (args && *args)
	{
		while ((*args)[original_count] != NULL)
			original_count++;
	}
	res_array = calloc_tracked(original_count + 2, sizeof(char *),
			COMMAND_TRACK, shell);
	while (i < original_count)
	{
		res_array[i] = (*args)[i];
		i++;
	}
	res_array[i] = str;
	res_array[i + 1] = NULL;
	*args = res_array;
}


/**
 * @brief Ожидает завершения дочернего процесса и обрабатывает ошибки.
 *
 * Эта функция вызывает стандартную функцию wait для ожидания завершения
 * дочернего процесса. В случае ошибки (если wait возвращает -1), функция

	* вызывает exit_on_sys_error для вывода сообщения об ошибке и завершения программы.
 *
 * @param status Указатель на переменную,
	в которую будет записан статус завершения
 *        дочернего процесса.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return PID завершенного дочернего процесса, иначе завершает программу с
 *         сообщением об ошибке.
 */
pid_t	wait_for_child(int *status, t_shell *shell)
{
	pid_t	child_pid;
	bool	error_occurred;

	child_pid = wait(status);
	error_occurred = (child_pid == -1);
	if (error_occurred)
		exit_on_sys_error("wait failed", errno, shell);
	return (child_pid);
}

/**
 * @brief Создает канал с помощью pipe и обрабатывает ошибки.
 *
 * Эта функция вызывает стандартную функцию pipe для создания канала.
 * В случае ошибки (если pipe возвращает -1), функция вызывает
 * exit_on_sys_error для вывода сообщения об ошибке и завершения программы.
 *
 * @param pipe_fds Массив из двух целых чисел, используемых для хранения
 *        файловых дескрипторов канала.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return 0 в случае успешного выполнения, иначе завершает программу с
 *         сообщением об ошибке.
 */
int	create_pipe(int pipe_fds[2], t_shell *shell)
{
	int		pipe_status;
	bool	error_occurred;

	pipe_status = pipe(pipe_fds);
	error_occurred = (pipe_status == -1);
	if (error_occurred)
		exit_on_sys_error("pipe failed", errno, shell);
	return (pipe_status);
}

/**
 * @brief Выполняет новую программу, заменяя текущий процесс.
 *
 * Эта функция вызывает стандартную библиотечную функцию execve, которая
 * выполняет новую программу, заменяя текущий процесс. В случае ошибки
 * execve возвращает -1 и устанавливает errno. Если execve возвращает -1,
 * эта функция вызывает exit_on_sys_error для вывода сообщения об ошибке и
 * завершения программы.
 *
 * @param path Путь к исполняемому файлу.
 * @param args Массив аргументов для исполняемого файла.
 * @param env Массив переменных окружения для нового процесса.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Эта функция не возвращает значение при успешном выполнении,
 *         так как текущий процесс будет заменен новой программой. В случае
 *         ошибки возвращает -1.
 */
int	execute_program(const char *path, char *const args[], char *const env[],
		t_shell *shell)
{
	int		exec_status;
	bool	error_occurred;

	exec_status = execve(path, args, env);
	error_occurred = (exec_status == -1);
	if (error_occurred)
		exit_on_sys_error("execve failed", errno, shell);
	return (exec_status);
}

/**
 * @brief Создает новый процесс с помощью fork и обрабатывает ошибки.
 *
 * Эта функция вызывает стандартную функцию fork для создания нового
 * процесса. В случае ошибки (если fork возвращает -1), функция вызывает
 * exit_on_sys_error для вывода сообщения об ошибке и завершения программы.
 *
 * @param shell Указатель на структуру shell для управления процессом.
 * @return PID нового процесса в случае успешного выполнения, иначе завершает
 *         программу с сообщением об ошибке.
 */
pid_t	create_process(t_shell *shell)
{
	pid_t	process_id;
	bool	error_occurred;

	process_id = fork();
	error_occurred = (process_id == -1);
	if (error_occurred)
		exit_on_sys_error("fork failed", errno, shell);
	return (process_id);
}

/**
 * @brief Открывает каталог и обрабатывает ошибки.
 *

	* Эта функция пытается открыть каталог по указанному пути и проверяет наличие ошибок.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param path Путь к каталогу, который нужно открыть.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Указатель на структуру DIR, представляющую открытый каталог.
 */
DIR	*open_directory(const char *path, t_shell *shell)
{
	DIR	*directory;

	errno = 0;
	directory = opendir(path);
	if (!directory)
	{
		if (errno != 0)
			exit_on_sys_error("opendir failed", errno, shell);
	}
	return (directory);
}

/**
 * @brief Закрывает каталог с обработкой ошибок.
 *
 * Эта функция пытается закрыть каталог,
	на который указывает аргумент directory, и проверяет наличие ошибок.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param directory Указатель на структуру DIR, представляющую открытый каталог.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Код завершения закрытия каталога. 0 в случае успешного закрытия,
	-1 в случае ошибки.
 */
int	close_directory(DIR *directory, t_shell *shell)
{
	int		close_status;
	bool	error_occurred;

	errno = 0;
	close_status = closedir(directory);
	error_occurred = (close_status == -1 && errno != 0);
	if (error_occurred)
		exit_on_sys_error("failed to close directory", errno, shell);
	return (close_status);
}

/**
 * @brief Читает запись из каталога с обработкой ошибок.
 *
 * Эта функция пытается прочитать следующую запись из каталога,
	на который указывает аргумент directory, и проверяет наличие ошибок.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param directory Указатель на структуру DIR, представляющую открытый каталог.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Указатель на структуру dirent, представляющую запись в каталоге.
 */
struct dirent	*read_directory(DIR *directory, t_shell *shell)
{
	struct dirent	*entry;
	bool			error_occurred;

	errno = 0;
	entry = readdir(directory);
	error_occurred = (entry == NULL && errno != 0);
	if (error_occurred)
		exit_on_sys_error("failed to read directory entry", errno, shell);
	return (entry);
}

/**
 * @brief Открывает файл с заданными флагами и разрешениями.
 *

	* Эта функция открывает файл по указанному пути с заданными флагами и разрешениями.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param path Путь к файлу.
 * @param mode_flags Флаги режима открытия файла.
 * @param permissions Разрешения для создания файла.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Файловый дескриптор открытого файла.
 */
int	open_file(const char *path, int mode_flags, mode_t permissions,
		t_shell *shell)
{
	int		file_descriptor;
	bool	error_occurred;

	file_descriptor = open(path, mode_flags, permissions);
	error_occurred = (file_descriptor == -1);
	if (error_occurred)
		exit_on_sys_error("open failed", errno, shell);
	return (file_descriptor);
}

/**
 * @brief Закрывает файл.
 *
 * Эта функция закрывает файл по его файловому дескриптору.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param fd Файловый дескриптор закрываемого файла.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Статус операции закрытия файла (0 при успехе, -1 при ошибке).
 */
int	close_file(int fd, t_shell *shell)
{
	int		close_status;
	bool	error_occurred;

	close_status = close(fd);
	error_occurred = (close_status == -1);
	if (error_occurred)
		exit_on_sys_error("close failed", errno, shell);
	return (close_status);
}

/**
 * @brief Получает статус файла, обрабатывая ошибки.
 *

	* Эта функция получает статус файла по его пути и заполняет структуру file_stat.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param file_path Путь к файлу.
 * @param file_stat Указатель на структуру stat для хранения информации о файле.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Статус операции stat (0 при успехе, -1 при ошибке).
 */
int	get_file_status(const char *restrict file_path,
		struct stat *restrict file_stat, t_shell *shell)
{
	int		stat_status;
	bool	error_occurred;

	stat_status = stat(file_path, file_stat);
	error_occurred = (stat_status == -1);
	if (error_occurred)
		exit_on_sys_error("stat failed", errno, shell);
	return (stat_status);
}

/**
 * @brief Дублирует файловый дескриптор.
 *
 * Эта функция дублирует source_fd в target_fd.
 * В случае ошибки завершает программу с сообщением об ошибке.
 *
 * @param source_fd Исходный файловый дескриптор.
 * @param target_fd Целевой файловый дескриптор.
 * @param shell Указатель на структуру shell для управления процессом.
 * @return Статус операции dup2 (0 при успехе, -1 при ошибке).
 */
int	duplicate_fd(int source_fd, int target_fd, t_shell *shell)
{
	int		dup_status;
	bool	error_occurred;

	dup_status = dup2(source_fd, target_fd);
	error_occurred = (dup_status == -1);
	if (error_occurred)
		exit_on_sys_error("dup2 failed", errno, shell);
	return (dup_status);
}
