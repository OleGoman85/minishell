#include "minishell.h"

int	ft_env(t_cmd *cmd, t_shell *shell)
{
	int	arg_count;

	arg_count = 0;
	if (cmd->cmd_args)
	{
		while (cmd->cmd_args[arg_count])
			arg_count++;
	}
	if (arg_count > 1)
	{
		error_msg("env: ", NULL, "more arguments than expected", shell);
		return (EXIT_FAILURE);
	}
	display_ev_list(false, shell->ev_list, shell);
	return (EXIT_SUCCESS);
}


/**
 * ft_env - Выводит список переменных окружения.
 * 
 * Функция подсчитывает количество аргументов команды.
 * Если аргументов больше одного, выводит сообщение об ошибке и возвращает EXIT_FAILURE.
 * Если аргумент один, выводит список переменных окружения и возвращает EXIT_SUCCESS.
 */