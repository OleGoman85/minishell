
#include "minishell.h"

/**
 * build_node_logic
 * @brief Создает AST узел для логической операции и инициализирует его.
 *

	* Эта функция принимает два выражения (first_expr и second_expr) и тип логической операции (op).
 * Если хотя бы одно из выражений равно NULL,
	функция записывает синтаксическую ошибку и возвращает NULL.
 *
 * 1. Проверяет, что оба выражения (first_expr и second_expr) не равны NULL.
 * 2. Если хотя бы одно из выражений равно NULL,
	записывает синтаксическую ошибку и возвращает NULL.
 * 3. Выделяет память для нового узла типа LOGIC.
 * 4. Инициализирует новый узел,
	устанавливая его тип LOGIC и связывая логическую операцию (op)
 *    с полями first_expr и second_expr.
 * 5. Возвращает созданный узел.
 *
 * @param first_expr Указатель на узел, представляющий первое выражение.
 * @param op Тип логической операции.
 * @param second_expr Указатель на узел, представляющий второе выражение.
 * @param shell Структура оболочки,
	используемая для управления памятью и синтаксическими ошибками.

	* @return Указатель на созданный узел логической операции или NULL в случае ошиб��и.
 */
t_ast	*build_node_logic(t_ast *first_expr, t_tkn_type op, t_ast *second_expr,
		t_shell *shell)
{
	t_ast	*logic_node;

	if (first_expr && second_expr)
	{
		logic_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (logic_node)
		{
			logic_node->node_type = LOGIC;
			logic_node->node_content.logic.logic_op = op;
			logic_node->node_content.logic.first = first_expr;
			logic_node->node_content.logic.second = second_expr;
		}
		return (logic_node);
	}
	else
		return (record_synt_err(get_tkn_label(op), shell));
}

/**
 * build_node_cmd
 * @brief Создает AST узел для команды и инициализирует его.
 *
 * Эта функция принимает массив аргументов команды (cmd_args).
 * Выделяет память для нового узла типа CMD и инициализирует его.
 *
* 1. Выделяет память для нового узла типа CMD с использованием функции setup_node.
* 2. Инициализирует новый узел,
	устанавливая его тип CMD и связывая аргументы команды (cmd_args).
 * 3. Возвращает созданный узел.
 *
 * @param cmd_args Массив аргументов команды.
 * @param shell Структура оболочки, используемая для управления памятью.
 * @return Указатель на созданный узел команды или NULL в случае ошибки.
 */
t_ast	*build_node_cmd(char **cmd_args, t_shell *shell)
{
	t_ast	*cmd_node;

	cmd_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
	if (cmd_node)
	{
		cmd_node->node_type = CMD;
		cmd_node->node_content.cmd.cmd_args = cmd_args;
	}
	return (cmd_node);
}

/**
 * build_node_pipe
 * @brief Создает AST узел для конвейера (pipe) и инициализирует его.
 *
 * Эта функция принимает два узла (input_node и output_node), которые будут
 * соединены с помощью конвейера. Если оба узла равны NULL, функция
 * записывает синтаксическую ошибку и возвращает NULL.
 *
 * 1. Проверяет, что оба узла (input_node и output_node) не равны NULL.
 * 2. Если хотя бы один из узлов равен NULL,
	записывает синтаксическую ошибку и возвращает NULL.
 * 3. Выделяет память для нового узла типа PIPE.
 * 4. Инициализирует новый узел, устанавливая его тип PIPE и связывая входной
 *    и выходной узлы с полями input_side и output_side соответственно.
 * 5. Возвращает созданный узел.
 *
 * @param input_node Указатель на узел,
	представляющий входную сторону конвейера.
 * @param output_node Указатель на узел,
	представляющий выходную сторону конвейера.
 * @param shell Структура оболочки
 * @return Указатель на созданный узел конвейера или NULL в случае ошибки.
 */
t_ast	*build_node_pipe(t_ast *input_node, t_ast *output_node, t_shell *shell)
{
	t_ast	*pipe_node;

	if (input_node && output_node)
	{
		pipe_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (pipe_node)
		{
			pipe_node->node_type = PIPE;
			pipe_node->node_content.pipe.input_side = input_node;
			pipe_node->node_content.pipe.output_side = output_node;
		}
		return (pipe_node);
	}
	return (record_synt_err("|", shell));
}

/**
 * @brief Создает узел AST для редиректа.
 *
 * @param redir_type Тип редиректа.
 * @param token Токен, содержащий имя файла.
 * @param child_node Узел команды, к которому применяется редирект.
 * @param shell Указатель на структуру оболочки для управления памятью.

	* @return Указатель на созданный узел AST для редиректа или NULL в случае ошибки.
 */
t_ast	*build_node_redir(t_tkn_type redir_type, t_list *token,
		t_ast *child_node, t_shell *shell)
{
	t_ast	*redir_node;
	char	*filename;

	if (token && get_type(token) == T_TEXT)
	{
		filename = get_value(token);
		if (filename)
		{
			redir_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
			if (redir_node)
			{
				redir_node->node_type = REDIR;
				redir_node->node_content.redir.filename = filename;
				redir_node->node_content.redir.redir_type = redir_type;
				redir_node->node_content.redir.command = child_node;
			}
			return (redir_node);
		}
	}
	record_synt_err(get_tkn_label(redir_type), shell);
	return (NULL);
}

/**
 * @brief Создает узел AST для группирующей команды (в скобках).
 *
 * @param inner_node Узел AST, представляющий команду внутри скобок.
 * @param shell Указатель на структуру оболочки для управления памятью.

	* @return Указатель на созданный узел AST для группы команд или NULL в случае ошибки.
 */
t_ast	*build_node_brace(t_ast *inner_node, t_shell *shell)
{
	t_ast *brace_node;

	if (inner_node != NULL)
	{
		brace_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (brace_node)
		{
			brace_node->node_type = BRACE;
			brace_node->node_content.brace.command = inner_node;
		}
		return (brace_node);
	}
	record_synt_err("(", shell);
	return (NULL);
}