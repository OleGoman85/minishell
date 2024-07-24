#include "minishell.h"

static bool	check_var_name(char *str)
{
	char	*current_char;

	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	current_char = str + 1;
	while (*current_char != '\0')
	{
		if (!(ft_isalnum(*current_char) || *current_char == '_'))
			return (false);
		current_char++;
	}
	return (true);
}
static char	*extract_var_name(char *ev_str, char *equals_pos, bool append_mode,
		t_shell *shell)
{
	int		name_length;
	char	*var_name;

	name_length = equals_pos - append_mode - ev_str;
	var_name = ft_substr(ev_str, 0, name_length);
	return (manage_memory(var_name, COMMAND_TRACK, shell));
}

static void	process_export(char *ev_str, bool *has_invalid_name, t_shell *shell)
{
	char	*ev_name;
	char	*equals_pos;
	t_list	*ev_node;
	bool	append_mode;

	equals_pos = ft_strchr(ev_str, '=');
	append_mode = (ft_strnstr(ev_str, "+=", equals_pos - ev_str + 1) != NULL);
	ev_name = extract_var_name(ev_str, equals_pos, append_mode, shell);
	if (!check_var_name(ev_name))
	{
		error_msg("export: `", ev_str, "': not a valid identifier", shell);
		*has_invalid_name = true;
		return ;
	}
	ev_node = get_ev(ev_name, shell->ev_list);
	if (ev_node && equals_pos)
		change_ev_val(ev_node, equals_pos + 1, append_mode, shell);
	else if (!ev_node)
	{
		if (equals_pos)
			add_ev(ev_name, equals_pos + 1, &(shell->ev_list), shell);
		else
			add_ev(ev_name, NULL, &(shell->ev_list), shell);
	}
}

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	bool has_invalid_name;
	char **args;

	args = cmd->cmd_args + 1;
	if (!*args)
		return (display_ev_list(true, shell->ev_list, shell));

	has_invalid_name = false;
	while (*args)
	{
		process_export(*args, &has_invalid_name, shell);
		args++;
	}
	if (has_invalid_name)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
1. check_var_name
Описание: Проверяет, является ли строка допустимым именем переменной.
Параметры:
char *str: Строка, содержащая имя переменной для проверки.
Алгоритм:
Проверяет, начинается ли строка с буквы или символа _.
Проверяет, содержат ли оставшиеся символы только буквы, цифры или символ _.
Результат: Возвращает true, если имя допустимо, и false в противном случае.

2. extract_var_name
Описание: Извлекает имя переменной из строки присваивания.
Параметры:
char *ev_str: Строка, содержащая присваивание в формате "имя=значение".
char *equals_pos: Позиция знака = в строке.
bool append_mode: Флаг, указывающий, используется ли +=.
t_shell *shell: Указатель на структуру оболочки для управления памятью.
Алгоритм:
Вычисляет длину имени переменной.
Извлекает подстроку, содержащую имя переменной.
Управляет выделенной памятью для подстроки.
Результат: Возвращает указатель на строку с именем переменной.

3. process_export
Описание: Обрабатывает строку присваивания для команды export.
Параметры:
char *ev_str: Строка, содержащая присваивание.
bool *has_invalid_name: Указатель на флаг, указывающий на наличие недопустимого имени.
t_shell *shell: Указатель на структуру оболочки.
Алгоритм:
Находит позицию знака = в строке.
Определяет, используется ли +=.
Извлекает имя переменной.
Проверяет валидность имени переменной.
Если имя недопустимо, выводит сообщение об ошибке и устанавливает флаг ошибки.
Проверяет, существует ли переменная в окружении и обновляет или добавляет её значение.
Результат: Обновляет или добавляет переменную в окружение.

4. ft_export
Описание: Реализует команду export, добавляя переменные в окружение.
Параметры:
t_cmd *cmd: Структура, содержащая аргументы команды.
t_shell *shell: Указатель на структуру оболочки.
Алгоритм:
Пропускает первый аргумент команды.
Если нет аргументов, выводит список переменных окружения.
Инициализирует флаг ошибки.
Обрабатывает каждый аргумент с помощью process_export.
Если были ошибки в именах, возвращает EXIT_FAILURE.
Результат: Возвращает EXIT_SUCCESS, если команда выполнена успешно, и EXIT_FAILURE в случае ошибок
*/