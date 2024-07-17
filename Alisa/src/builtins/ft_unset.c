#include "minishell.h"


int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	t_list *ev;
	char **args;

	args = cmd->cmd_args + 1;
	while (*args != NULL)
	{
		ev = get_ev(*args, shell->ev_list);
		if (ev != NULL)
			del_node(&ev, &(shell->ev_list), free_ev, true);
		args++;
	}
	return (EXIT_SUCCESS);
}
// Пропуск первого аргумента:

// args = cmd->cmd_args + 1; — Начинает обработку со второго элемента в списке аргументов команды.
// Цикл по аргументам:

// Проходит по каждому аргументу в cmd->cmd_args, пока не встретит NULL.
// Поиск переменной окружения:

// ev = get_ev(*args, shell->ev_list); — Ищет переменную окружения с именем, указанным в текущем аргументе.
// Удаление переменной:

// Если переменная найдена, она удаляется из списка shell->ev_list с помощью del_node(&ev, &(shell->ev_list), free_ev, true);.
// Переход к следующему аргументу:

// args++; — Переходит к следующему аргументу в списке.
// Возврат успешного завершения:

// return EXIT_SUCCESS; — Возвращает EXIT_SUCCESS после завершения обработки всех аргументов.
// Вспомогательные функции
// get_ev — Ищет переменную окружения по имени.
// del_node — Удаляет узел из списка и освобождает память.
// free_ev — Освобождает память, выделенную под переменную окружения.