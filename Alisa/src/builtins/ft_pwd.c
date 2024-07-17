
#include "minishell.h"

int	ft_pwd(t_cmd *cmd, t_shell *shell)
{
	t_list	*pwd_ev;
	char	*pwd_value;
	char	*dir_path;

	(void)cmd;
	pwd_ev = get_ev("PWD", shell->ev_list);
	pwd_value = get_ev_value(pwd_ev);
	if (pwd_value)
		write_and_track(pwd_value, STDOUT_FILENO, shell);
	else
	{
		dir_path = getcwd(NULL, 0);
		if (!dir_path)
		{
			exit_on_sys_error("getcwd", errno, shell);
			return (EXIT_FAILURE);
		}
		manage_memory(dir_path, COMMAND_TRACK, shell);
		write_and_track(dir_path, STDOUT_FILENO, shell);
	}
	write_and_track("\n", STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}

/**
 * ft_pwd - Выводит текущий рабочий каталог.
 *
 * Функция пытается получить значение переменной окружения PWD.
 * Если значение найдено, выводит его.
 * Если значение не найдено,
	вызывает getcwd для получения текущего рабочего каталога.
 * В случае ошибки getcwd, завершает выполнение с ошибкой.
 * В конце функции всегда добавляет новую строку и возвращает EXIT_SUCCESS.
 */