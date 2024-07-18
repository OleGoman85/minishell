#include "minishell.h"


/**
 * @brief Подсчитывает количество непустых аргументов в массиве строк.
 *
 * @param args Массив строковых аргументов.
 * @return Количество непустых аргументов.
 */
static int	count_valid_args(char **args)
{
	int	total;
	int	valid_args;

	total = 0;
	valid_args = 0;
	while (args && args[total])
	{
		if (ft_strlen(args[total]) > 0)
			valid_args++;
		total++;
	}
	return (valid_args);
}

/**
 * @brief Получает текущую директорию.
 *
 * Эта функция выделяет память для буфера, затем вызывает функцию getcwd,
 * чтобы получить текущую директорию. Если getcwd возвращает NULL, то
 * функция обрабатывает ошибку. Если getcwd успешен, то путь к текущей
 * директории сохраняется в current_directory.
 *
 * @param buffer_size Размер буфера для хранения пути.
 * @param current_directory Указатель на строку для хранения текущего пути.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успеха, иначе EXIT_FAILURE.
 */
static int	fetch_current_dir(size_t buffer_size, char **current_pwd,
		t_shell *shell)
{
	char	*buffer;
	char	*directory;

	buffer = calloc_tracked(PATH_MAX, 1, COMMAND_TRACK, shell);
	if (buffer == NULL)
	{
		exit_on_sys_error("calloc_tracked: ", errno, shell);
		return (EXIT_FAILURE);
	}
	directory = getcwd(buffer, buffer_size);
	if (directory != NULL)
	{
		*current_pwd = directory;
		return (EXIT_SUCCESS);
	}
	if (errno == ENOENT)
	{
		error_msg("cd: unable to retrieve current directory: ",
			"getcwd: no access to directories", strerror(errno), shell);
		change_ev_val(get_ev("PWD", shell->ev_list), "/.", true, shell);
	}
	else
		exit_on_sys_error("getcwd: ", errno, shell);
	return (EXIT_FAILURE);
}


/**
 * @brief Обновляет пути директорий (OLDPWD и PWD).
 *
 * Эта функция обновляет значения переменных окружения OLDPWD и PWD.
 * Сначала она получает текущую директорию, затем обновляет OLDPWD значением
 * старой директории, и наконец обновляет PWD значением текущей директории.
 *
 * @param old_directory Значение предыдущей директории.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успеха, иначе EXIT_FAILURE.
 */
static int	update_dir_paths(char *oldpwd_val, t_shell *shell)
{
	char	*current_pwd;
	t_list	*oldpwd_ev;
	t_list	*pwd_ev;
	int		dir_status;

	current_pwd = NULL;
	oldpwd_ev = get_ev("OLDPWD", shell->ev_list);
	pwd_ev = get_ev("PWD", shell->ev_list);
	if (oldpwd_ev)
		change_ev_val(oldpwd_ev, oldpwd_val, false, shell);
	else
		add_ev("OLDPWD", oldpwd_val, &(shell->ev_list), shell);
	dir_status = fetch_current_dir(PATH_MAX, &current_pwd, shell);
	if (dir_status != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (current_pwd)
	{
		if (pwd_ev)
			change_ev_val(pwd_ev, current_pwd, false, shell);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Меняет текущую директорию.
 *
 * Эта функция изменяет текущую директорию на target_directory.
 * Если операция успешна, она обновляет пути директорий.
 * В случае ошибки возвращает сообщение об ошибке.
 *
 * @param cmd Указатель на структуру команды.
 * @param target_directory Путь к целевой директории.
 * @param old_directory Значение предыдущей директории.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успеха, иначе EXIT_FAILURE.
 */
static int	change_current_dir(t_cmd *cmd, char *target_path, char *oldpwd_val,
		t_shell *shell)
{
	char	*error_detail;
	char	*output;
	int		update_status;

	if (chdir(target_path) == -1)
	{
		error_detail = strjoin_tracked(": ", strerror(errno), COMMAND_TRACK,
				shell);
		return (error_msg("cd: ", target_path, error_detail, shell));
	}
	update_status = update_dir_paths(oldpwd_val, shell);
	if (update_status != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (cmd->cmd_args[1] && ft_strcmp(cmd->cmd_args[1], "-") == 0)
	{
		output = strjoin_tracked(target_path, "\n", COMMAND_TRACK, shell);
		write_and_track(output, STDOUT_FILENO, shell);
	}
	return (EXIT_SUCCESS);
}
/**
 * @brief Определяет целевую директорию для команды cd.
 *
 * Эта функция анализирует аргументы команды и определяет целевую директорию,
 * в которую нужно перейти. Она учитывает такие случаи, как отсутствие аргументов,
 * пустой аргумент, символ '-' для перехода в предыдущую директорию и другие.
 *
 * @param cmd Указатель на структуру команды.
 * @param target_directory Указатель на строку для хранения пути целевой директории.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успеха, иначе EXIT_FAILURE.
 */
static int	get_target_dir(t_cmd *cmd, char **directory, t_shell *shell)
{
	t_list	*home_ev;
	t_list	*oldpwd_ev;

	if (cmd->cmd_args[1] == NULL)
	{
		home_ev = get_ev("HOME", shell->ev_list);
		if (home_ev == NULL)
			return (error_msg("cd :", NULL,
					"HOME environment variable is not set", shell));
		*directory = get_ev_value(home_ev);
	}
	else if (ft_strlen(cmd->cmd_args[1]) == 0)
		*directory = get_ev_value(get_ev("PWD", shell->ev_list));
	else if (ft_strcmp(cmd->cmd_args[1], "-") == 0)
	{
		oldpwd_ev = get_ev("OLDPWD", shell->ev_list);
		if (oldpwd_ev == NULL)
			return (error_msg("cd :", NULL,
					"OLDPWD environment variable is not set", shell));
		*directory = strdup_tracked(get_ev_value(oldpwd_ev), COMMAND_TRACK,
				shell);
	}
	else
		*directory = cmd->cmd_args[1];
	return (EXIT_SUCCESS);
}


/**
 * @brief Основная функция cd, которая изменяет текущую директорию.
 *
 * Эта функция является основной функцией команды cd. Она обрабатывает аргументы команды,
 * определяет целевую директорию, затем пытается перейти в эту директорию, обновляя пути
 * директорий в случае успеха. Если аргументов больше двух, лишние аргументы игнорируются.
 *
 * @param cmd Указатель на структуру команды.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успеха, иначе EXIT_FAILURE.
 */
int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	char	*target_dir;
	char	*prev_dir;
	int		result;

	target_dir = NULL;
	prev_dir = NULL;
	result = EXIT_FAILURE;
	if (cmd->cmd_args[0] == NULL)
		return (EXIT_FAILURE);
	if (count_valid_args(cmd->cmd_args) > 2)
		cmd->cmd_args[2] = NULL;
	prev_dir = get_ev_value(get_ev("PWD", shell->ev_list));
	if (get_target_dir(cmd, &target_dir, shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	result = change_current_dir(cmd, target_dir, prev_dir, shell);
	return (result);
}
