
#include "minishell.h"

/**
 * @brief Инициализирует массив встроенных команд.
 *
 * Эта функция заполняет массив встроенных команд `commands` именами
 * команд и соответствующими функциями. Массив завершается элементом с NULL.
 *
 * @param commands Указатель на массив структур команд, который будет
 * заполнен встроенными командами и функциями.
 */
void	init_builtins(t_bltn_cmd *commands)
{
	commands[0].cmd_name = "echo";
	commands[0].cmd_func = ft_echo;
	commands[1].cmd_name = "cd";
	commands[1].cmd_func = ft_cd;
	commands[2].cmd_name = "pwd";
	commands[2].cmd_func = ft_pwd;
	commands[3].cmd_name = "export";
	commands[3].cmd_func = ft_export;
	commands[4].cmd_name = "unset";
	commands[4].cmd_func = ft_unset;
	commands[5].cmd_name = "env";
	commands[5].cmd_func = ft_env;
	commands[6].cmd_name = "exit";
	commands[6].cmd_func = ft_exit;
	commands[7].cmd_name = NULL;
	commands[7].cmd_func = NULL;
}

/**
 * @brief Находит и возвращает функцию встроенной команды по ее имени.
 *
 * Эта функция ищет встроенную команду по ее имени в массиве команд.
 * Если команда найдена, возвращается указатель на соответствующую функцию.
 * Если команда не найдена, возвращается NULL.
 *
 * @param command Имя команды, которую нужно найти.
 * @return Указатель на функцию встроенной команды, или NULL, если команда
 * не найдена.
 */
t_bltn_func	fetch_builtin_cmd(char *command)
{
	t_bltn_cmd	commands[8];
	int			index;
	int			name_found;
	t_bltn_func	result_func;

	index = 0;
	name_found = 0;
	result_func = NULL;
	init_builtins(commands);
	while (commands[index].cmd_name != NULL && !name_found)
	{
		if (ft_strcmp(command, commands[index].cmd_name) == 0)
		{
			result_func = commands[index].cmd_func;
			name_found = 1;
		}
		index++;
	}
	return (result_func);
}
