#include "minishell.h"

/**
 * @brief Обновляет переменные окружения PWD и OLDPWD.
 *
 * Эта функция обновляет переменные окружения PWD и OLDPWD, используя
 * текущий рабочий каталог. Если текущий каталог недоступен, обрабатываются
 * соответствующие ошибки.
 *
 * @param current_pwd Указатель на строку, содержащую текущий рабочий каталог.
 * @param oldpwd_val Значение переменной OLDPWD.
 * @param shell Указатель на структуру оболочки для управления состоянием.
 * @return Код завершения выполнения функции (EXIT_SUCCESS при успехе,
	EXIT_FAILURE при ошибке).
 */
static int	update_env_vars(char **current_pwd, char *oldpwd_val,
		t_shell *shell)
{
	char	*buffer;
	t_list	*ev;

	buffer = calloc_tracked(PATH_MAX, 1, COMMAND_TRACK, shell);
	if (!buffer)
		exit_on_sys_error("calloc_tracked: ", errno, shell);
	if (!(*current_pwd = getcwd(buffer, PATH_MAX)))
	{
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
	if ((ev = get_ev("OLDPWD", shell->ev_list)))
		change_ev_val(ev, oldpwd_val, false, shell);
	else
		add_ev("OLDPWD", oldpwd_val, &(shell->ev_list), shell);
	if ((ev = get_ev("PWD", shell->ev_list)))
		change_ev_val(ev, *current_pwd, false, shell);
	return (EXIT_SUCCESS);
}

/**
 * @brief Обрабатывает аргументы команды cd и определяет целевой каталог.
 *
 * Эта функция анализирует аргументы команды cd и определяет целевой каталог
 * для смены текущего рабочего каталога. Если аргументы пусты, используется
 * значение переменной окружения HOME. Если аргумент равен "-", используется
 * значение переменной OLDPWD.
 *
 * @param cmd Указатель на структуру команды.
 * @param target_dir Указатель на строку, содержащую целевой каталог.
 * @param shell Указатель на структуру оболочки для управления состоянием.
 * @return Код завершения выполнения функции (EXIT_SUCCESS при успехе,
	EXIT_FAILURE при ошибке).
 */
static int	process_cd_args(t_cmd *cmd, char **target_dir, t_shell *shell)
{
	t_list	*env_var;

	if (cmd->cmd_args[1] == NULL)
	{
		env_var = get_ev("HOME", shell->ev_list);
		if (env_var == NULL)
			return (error_msg("cd: ", NULL,
					"HOME environment variable is not set", shell));
		*target_dir = get_ev_value(env_var);
	}
	else if (ft_strcmp(cmd->cmd_args[1], "-") == 0)
	{
		env_var = get_ev("OLDPWD", shell->ev_list);
		if (env_var == NULL)
			return (error_msg("cd: ", NULL,
					"OLDPWD environment variable is not set", shell));
		*target_dir = strdup_tracked(get_ev_value(env_var), COMMAND_TRACK,
				shell);
	}
	else
		*target_dir = cmd->cmd_args[1];
	return (EXIT_SUCCESS);
}

/**
 * @brief Изменяет текущий рабочий каталог, используя команду cd.
 *
 * Эта функция выполняет команду cd, изменяя текущий рабочий каталог.
 * Если команда имеет больше одного аргумента, возвращает ошибку.
 * Сохраняет предыдущий рабочий каталог и выполняет необходимые проверки
 * и изменения каталога.
 *
 * @param cmd Указатель на структуру с информацией о команде.
 * @param shell Указатель на структуру оболочки для управления состоянием.
 * @return Код завершения выполнения команды.
 */
int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	char	*current_pwd;

	current_pwd = NULL;
	if (cmd->cmd_args[1] != NULL && cmd->cmd_args[2] != NULL)
		return (error_msg("cd: ", NULL, "too many parameters passed", shell));
	if (process_cd_args(cmd, &current_pwd, shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (chdir(current_pwd) == -1)
		return (error_msg("cd: ", current_pwd, strjoin_tracked(": ",
					strerror(errno), COMMAND_TRACK, shell), shell));
	if (update_env_vars(&current_pwd, get_ev_value(get_ev("PWD",
					shell->ev_list)), shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (cmd->cmd_args[1] && ft_strcmp(cmd->cmd_args[1], "-") == 0)
		write_and_track(strjoin_tracked(current_pwd, "\n", COMMAND_TRACK,
				shell), STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}
