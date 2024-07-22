#include "minishell.h"

static char	get_escape_replacement(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 'r')
		return ('\r');
	if (c == 't')
		return ('\t');
	return (c);
}
/**
 * @brief Обрабатывает escape-последовательности в строке.
 *
 * Эта функция принимает строку, обрабатывает все escape-последовательности
 * и возвращает новую строку с заменёнными символами. Память для новой строки
 * выделяется с использованием функции calloc_tracked.
 *
 * @param arg Входная строка для обработки.
 * @param shell Указатель на структуру данных оболочки.
 * @return Новая строка с заменёнными escape-последовательностями.
 */
static char	*process_escape_sequences(const char *arg, t_shell *shell)
{
	char	*result;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(arg);
	i = 0;
	j = 0;
	result = calloc_tracked(len + 1, sizeof(char), COMMAND_TRACK, shell);
	if (!result)
		exit_on_sys_error("calloc_tracked: ", errno, shell);
	while (arg[i])
	{
		if (arg[i] == '\\' && arg[i + 1])
			result[j++] = get_escape_replacement(arg[++i]);
		else
			result[j++] = arg[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}


/**
 * @brief Проверяет наличие флага в аргументах команды.
 *
 * Эта функция проверяет, есть ли флаг (символ) среди аргументов команды,
 * начиная с заданного индекса. Если флаг найден, индекс увеличивается,
 * и функция возвращает true.
 *
 * @param cmd_args Массив строковых аргументов команды.
 * @param index Указатель на индекс, с которого начинается проверка.
 * @param flag Символ флага для поиска.
 * @return true, если флаг найден, иначе false.
 */
static bool	has_flag(char **cmd_args, int *index, char flag)
{
	bool	flag_found;
	int		char_pos;

	flag_found = false;
	while (cmd_args[*index] && cmd_args[*index][0] == '-')
	{
		char_pos = 1;
		while (cmd_args[*index][char_pos] == flag)
			char_pos++;
		if (cmd_args[*index][char_pos] == '\0' && char_pos > 1)
		{
			flag_found = true;
			(*index)++;
		}
		else
			return (flag_found);
	}
	return (flag_found);
}


/**
 * @brief Выполняет команду echo с обработкой escape-последовательностей.
 *
 * Эта функция выполняет команду echo, обрабатывая escape-последовательности
 * в аргументах команды и выводя их на стандартный вывод. Если аргумент
 * команды содержит флаг -n, вывод новой строки в конце отключается.
 *
 * @param cmd Указатель на структуру команды.
 * @param shell Указатель на структуру данных оболочки.
 * @return EXIT_SUCCESS в случае успешного выполнения команды.
 */
int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	bool	add_newline;
	int		i;
	char	*processed_arg;

	add_newline = true;
	i = 1;
	if (has_flag(cmd->cmd_args, &i, 'n'))
		add_newline = false;
	while (cmd->cmd_args[i])
	{
		processed_arg = process_escape_sequences(cmd->cmd_args[i], shell);
		write_and_track(processed_arg, STDOUT_FILENO, shell);
		if (cmd->cmd_args[i + 1])
			write_and_track(" ", STDOUT_FILENO, shell);
		i++;
	}
	if (add_newline)
		write_and_track("\n", STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}
