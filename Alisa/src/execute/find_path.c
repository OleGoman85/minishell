
#include "minishell.h"

/**
 * @brief Генерирует массив путей из переменной окружения PATH.
 *
 * Функция получает значение переменной окружения PATH и разбивает его
 * на массив строк, представляющих различные пути. Если в PATH включен
 * текущий каталог (".") в качестве одного из путей, он добавляется
 * к массиву путей.
 *
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Массив строк, содержащих пути из переменной окружения PATH.
 */
char	**generate_paths(t_shell *shell)
{
	char	**dir_paths;
	char	*env_paths;
	bool	includes_dot;

	includes_dot = false;
	dir_paths = NULL;
	env_paths = get_ev_value(get_ev("PATH", shell->ev_list));
	if (!env_paths)
		env_paths = strdup_tracked(":", COMMAND_TRACK, shell);
	dir_paths = split_tracked(env_paths, ':', COMMAND_TRACK, shell);
	if (env_paths[0] == ':' || env_paths[ft_strlen(env_paths) - 1] == ':'
		|| ft_strnstr(env_paths, "::", ft_strlen(env_paths)) != NULL)
		includes_dot = true;
	if (includes_dot)
		append_str(&dir_paths, strdup_tracked(".", COMMAND_TRACK, shell),
			shell);
	return (dir_paths);
}

/**
 * @brief Находит путь к исполняемому файлу.
 *
 * Функция проверяет, является ли команда заданным путем или нужно
 * искать её в путях переменной окружения PATH. Если команда содержит
 * '/', она проверяет доступность файла по указанному пути.
 * В противном случае, функция ищет команду в PATH.
 *
 * @param cmd Имя команды или путь к исполняемому файлу.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Путь к исполняемому файлу или NULL, если команда не найдена.
 */
char	*find_executable_path(char *cmd, t_shell *shell)
{
	struct stat	file_info;

	if (ft_strncmp(cmd, "", 1) == 0)
		exit_on_error("command not found", cmd, 127, shell);
	else if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == -1)
		{
			if (errno == EACCES)
				exit_on_error("access denied", cmd, 126, shell);
			else if (errno == ENOENT)
				exit_on_error("No such file or directory", cmd, 127, shell);
			else
				exit_on_sys_error(cmd, errno, shell);
		}
		get_file_status(cmd, &file_info, shell);
		if (S_ISDIR(file_info.st_mode))
			exit_on_error("Is a directory", cmd, 126, shell);
		return (cmd);
	}
	else if (ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "..", 2) == 0)
		exit_on_error("command not found", cmd, 127, shell);
	else
		return (locate_executable(cmd, shell));
	return (NULL);
}
/**
 * @brief Ищет исполняемый файл в путях, указанных в переменной PATH.
 *
 * Функция перебирает пути в переменной PATH и пытается найти
 * исполняемый файл. Если файл найден и доступен для выполнения,
 * функция возвращает его полный путь. В противном случае, возвращает
 * сообщение об ошибке.
 *
 * @param bin_name Имя исполняемого файла.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Полный путь к исполняемому файлу или NULL, если команда не найдена.
 */
char	*locate_executable(char *bin_name, t_shell *sh)
{
	char	*full_path;
	bool	access_error;
	char	**dir_paths;
	int		i;

	dir_paths = generate_paths(sh);
	if (!dir_paths)
		exit_on_error("command not found in PATH", bin_name, 127, sh);
	access_error = false;
	i = -1;
	while (dir_paths[++i] != NULL)
	{
		full_path = strjoin_tracked(strjoin_tracked(dir_paths[i], "/",
					COMMAND_TRACK, sh), bin_name, COMMAND_TRACK, sh);
		if (access(full_path, X_OK) == EXIT_SUCCESS)
			return (full_path);
		else if (errno == EACCES)
			access_error = true;
		else if (errno != ENOENT)
			exit_on_sys_error("error accessing path", errno, sh);
	}
	if (access_error)
		exit_on_error("access denied", bin_name, 126, sh);
	exit_on_error("command not found", bin_name, 127, sh);
	return (NULL);
}
