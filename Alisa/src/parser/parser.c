
#include "minishell.h"

/**
 * get_tkn_label
 * @brief Возвращает строковое представление типа токена.
 *

	* Функция `get_tkn_label` принимает тип токена `tkn_type` и возвращает его строковое представление.
 * В зависимости от значения `tkn_type` возвращается соответствующая строка.
 * Если тип токена не распознан, возвращается строка "unsupported token".
 *
 * @param tkn_type Тип токена (перечисление `t_tkn_type`).
 * @return Строковое представление типа токена. Если тип токена не распознан,
 * возвращается "unsupported token".
 */
char	*get_tkn_label(t_tkn_type tkn_type)
{
	char	*result;

	result = "unsupported token";
	if (tkn_type == T_TEXT)
		result = "text";
	else if (tkn_type == T_HDOC)
		result = "<<";
	else if (tkn_type == T_APPEND)
		result = ">>";
	else if (tkn_type == T_PIPE)
		result = "|";
	else if (tkn_type == T_OR)
		result = "||";
	else if (tkn_type == T_BRACE_START)
		result = "(";
	else if (tkn_type == T_BRACE_END)
		result = ")";
	else if (tkn_type == T_AND)
		result = "&&";
	else if (tkn_type == T_INPUT)
		result = "<";
	else if (tkn_type == T_OUTPUT)
		result = ">";
	return (result);
}

/**
 * get_value
 * @brief Возвращает значение токена из узла списка.
 *
 * Функция `get_value` принимает указатель на узел списка `tkn`.
 * Она выполняет следующие шаги:
 * 1. Инициализирует переменную `value` значением `NULL`.
 * 2. Проверяет, что узел списка `tkn` не равен `NULL`.
 * 3. Извлекает содержимое узла списка (предполагая, что это структура `t_tkn`).

	* 4. Извлекает значение токена из структуры `t_tkn` и сохраняет его в переменной `value`.
 * 5. Возвращает значение токена.
 *
 * @param tkn Указатель на узел списка токенов.
 * @return Указатель на строку, содержащую значение токена, или `NULL`,
	если узел списка пустой.
 */
char	*get_value(t_list *tkn)
{
	char	*value;
	t_tkn	*tkn_content;

	value = NULL;
	if (tkn != NULL)
	{
		tkn_content = (t_tkn *)tkn->content;
		value = tkn_content->value;
	}
	return (value);
}

/**
 * get_type
 * @brief Возвращает тип токена из узла списка.
 *
 * Функция `get_type` принимает указатель на узел списка `tkn_node`.
 * Она выполняет следующие шаги:
 * 1. Извлекает содержимое узла списка (предполагая, что это структура `t_tkn`).
 * 2. Возвращает тип токена, который хранится в структуре `t_tkn`.
 *
 * @param tkn_node Указатель на узел списка токенов.
 * @return Значение типа `t_tkn_type`, представляющее тип токена.
 */
t_tkn_type	get_type(t_list *tkn_node)
{
	t_tkn	*tkn_content;

	tkn_content = (t_tkn *)tkn_node->content;
	return (tkn_content->type);
}

/** parse_cmd
 * @brief Парсит команду и создает AST узел команды.
 *
 * Функция `parse_cmd` принимает список токенов `tokens` и структуру `shell`.
 * Она выполняет следующие шаги:
 * 1. Инициализирует переменные: `arg_count` для подсчета аргументов,
	`i` для итерации,
	`cur_tkn` для текущего токена и `cmd_node` для узла команды.
 * 2. Проходит по токенам и подсчитывает количество аргументов команды,
	пока не встретит токен, не являющийся текстом.
 * 3. Если не найдено ни одного аргумента (т.е. `arg_count` равно 0),
	возвращает `NULL`.
 * 4. Выделяет память для массива аргументов команд (`args`),
	включая место для завершающего `NULL`.

	* 5. Заполняет массив `args` значениями токенов и продвигает указатель на текущий токен (`tokens`).
 * 6. Создает AST узел команды с помощью `build_node_cmd` и возвращает его.
 *
 * @param tokens Указатель на список токенов.
 * @param shell Структура оболочки.
 * @return Указатель на AST узел команды или `NULL`, если аргументы не найдены.
 */
t_ast	*parse_cmd(t_list **tokens, t_shell *shell)
{
	int		arg_count;
	char	**args;
	int		i;
	t_list	*cur_tkn;
	t_ast	*cmd_node;

	arg_count = 0;
	i = -1;
	cur_tkn = *tokens;
	while (cur_tkn && get_type(cur_tkn) == T_TEXT)
	{
		arg_count++;
		cur_tkn = cur_tkn->next;
	}
	if (arg_count == 0)
		return (NULL);
	args = calloc_tracked(arg_count + 1, sizeof(char *), COMMAND_TRACK, shell);
	while (++i < arg_count)
	{
		args[i] = get_value(*tokens);
		*tokens = (*tokens)->next;
	}
	args[arg_count] = NULL;
	cmd_node = build_node_cmd(args, shell);
	return (cmd_node);
}


/**
 * @brief Парсит логические выражения и строит соответствующее дерево AST.
 *
 * Функция parse_logic выполняет разбор логических операторов (&& и ||) в 
 * команде и строит дерево AST, которое представляет 
 * собой логическую структуру команды. Она рекурсивно разбирает выражения 
 * с логическими операторами и соединяет их в дерево.
 *
 * @param tkn_list Указатель на список токенов, представляющих команду.
 * @param shell Указатель на структуру shell, содержащую информацию о состоянии оболочки.
 * 
 * @return Указатель на корень AST, представляющий логическое выражение.
 *
 * Функция выполняет следующие шаги:
 * 1. Парсит первое подвыражение с помощью функции parse_pipe и сохраняет его в first_expr.
 * 2. В цикле проверяет, есть ли еще токены для разбора и являются ли они логическими операторами (&& или ||).
 * 3. Если текущий токен является логическим оператором, сохраняет его тип в operator_type и переходит к следующему токену.
 * 4. Парсит второе подвыражение с помощью функции parse_pipe и сохраняет его в second_expr.
 * 5. Если второе подвыражение не удалось распарсить, возвращает первое подвыражение.
 * 6. Создает новый узел логического оператора с помощью функции build_node_logic, передавая в нее первое подвыражение, тип оператора и второе подвыражение.
 * 7. Если создание узла не удалось, возвращает первое подвыражение.
 * 8. Обновляет first_expr на новый логический узел и продолжает цикл.
 * 9. Возвращает корень дерева AST, представляющий логическое выражение.
 */
t_ast	*parse_logic(t_list **tkn_list, t_shell *shell)
{
	t_ast		*first_expr; //left expression
	t_ast		*second_expr; //right expression
	t_ast		*logical_expr;
	t_tkn_type	operator_type;

	first_expr = parse_pipe(tkn_list, shell);
	while (tkn_list && *tkn_list)
	{
		operator_type = get_type(*tkn_list);
		if (operator_type != T_AND && operator_type != T_OR)
			break ;
		*tkn_list = (*tkn_list)->next;
		second_expr = parse_pipe(tkn_list, shell);
		if (!second_expr)
			return (first_expr);
		logical_expr = build_node_logic(first_expr, operator_type, second_expr,
				shell);
		if (!logical_expr)
			return (first_expr);
		first_expr = logical_expr;
	}
	return (first_expr);
}
/**
 * @brief Парсит команды с пайпами и строит соответствующее дерево AST.
 *
 * Функция parse_pipe выполняет разбор операторов пайпа (|) в команде и строит 
 * дерево AST (Abstract Syntax Tree), которое представляет собой цепочку команд, 
 * соединенных через пайпы. Она рекурсивно разбирает команды с пайпами и 
 * соединяет их в дерево.
 *
 * @param tkn_list Указатель на список токенов, представляющих команду.
 * @param shell Указатель на структуру shell, содержащую информацию о состоянии оболочки.
 * 
 * @return Указатель на корень AST, представляющий цепочку команд с пайпами.
 *
 * Функция выполняет следующие шаги:
 * 1. Парсит первое подвыражение с редиректами с помощью функции process_redir и сохраняет его в input_side.
 * 2. В цикле проверяет, есть ли еще токены для разбора и являются ли они оператором пайпа (|).
 * 3. Если текущий токен является оператором пайпа, переходит к следующему токену.
 * 4. Парсит второе подвыражение с редиректами с помощью функции process_redir и сохраняет его в output_side.
 * 5. Если второе подвыражение не удалось распарсить, возвращает первое подвыражение.
 * 6. Создает новый узел пайпа с помощью функции build_node_pipe, передавая в нее первое и второе подвыражения.
 * 7. Если создание узла не удалось, возвращает первое подвыражение.
 * 8. Обновляет input_side на новый узел пайпа и продолжает цикл.
 * 9. Возвращает корень дерева AST, представляющий цепочку команд с пайпами.
 */
t_ast	*parse_pipe(t_list **tkn_list, t_shell *shell)
{
	t_ast	*input_side; //left expression
	t_ast	*output_side; //right expression
	t_ast	*pipeline_expr;

	input_side = process_redir(tkn_list, shell);
	while (tkn_list && *tkn_list)
	{
		if (get_type(*tkn_list) != T_PIPE)
			break ;
		*tkn_list = (*tkn_list)->next;
		output_side = process_redir(tkn_list, shell);
		if (!output_side)
			return (input_side);
		pipeline_expr = build_node_pipe(input_side, output_side, shell);
		if (!pipeline_expr)
			return (input_side);
		input_side = pipeline_expr;
	}
	return (input_side);
}

/** parse_brace
 * @brief Разбирает токены, заключенные в скобки,
	и создает соответствующие узлы AST.
 *
 * Эта функция обрабатывает токены,
	которые заключены в скобки (T_BRACE_START и T_BRACE_END).
 * Она проверяет наличие начальной скобки,
	затем вызывает функцию `parse_logic_and_pipe` для разбора содержимого скобок.

	* После этого она проверяет наличие закрывающей скобки. Если обнаруживается синтаксическая ошибка,
	функция фиксирует её и возвращает NULL.
 *
 * @param tokens Указатель на список токенов.
 * @param shell Указатель на структуру shell.
 * @return t_ast* Указатель на узел AST, представляющий содержимое скобок,
	или NULL в случае синтаксической ошибки.
 */
t_ast	*parse_brace(t_list **tokens, t_shell *shell)
{
	t_ast	*inner_expr;
	t_list	*cur_tkn;

	inner_expr = NULL;
	cur_tkn = *tokens;
	if (cur_tkn && get_type(cur_tkn) == T_BRACE_START)
	{
		cur_tkn = cur_tkn->next;
		inner_expr = parse_logic(&cur_tkn, shell);
		if (!cur_tkn)
			return (record_synt_err("\\n", shell));
		if (get_type(cur_tkn) == T_BRACE_END)
		{
			cur_tkn = cur_tkn->next;
			if (cur_tkn && get_type(cur_tkn) == T_TEXT)
				return (record_synt_err(get_value(cur_tkn), shell));
			*tokens = cur_tkn;
			return (build_node_brace(inner_expr, shell));
		}
		return (record_synt_err(get_value(cur_tkn), shell));
	}
	if (cur_tkn && get_type(cur_tkn) == T_BRACE_END)
		return (record_synt_err(")", shell));
	return (parse_cmd(tokens, shell));
}

/**
 * parse_tokens
 * @brief Парсит список токенов и создает синтаксическое дерево (AST).
 *
 * Функция `parse_tokens` принимает список токенов `tokens` и структуру `shell`.
 * Она выполняет следующие шаги:
 * 1. Вызывает `parse_logic_and_pipe`,
	чтобы создать синтаксическое дерево (AST) из токенов.
 * 2. Если после парсинга остались токены,
	записывает синтаксическую ошибку с первым неиспользованным токеном.
 * 3. Если в процессе парсинга обнаружена синтаксическая ошибка,
	отображает её и возвращает результат ошибки.
 * 4. Если ошибок нет, возвращает `EXIT_SUCCESS`.
 *
 * @param tokens Указатель на список токенов.
 * @param syntax_tree Указатель на указатель синтаксического дерева.
 * @param shell Структура оболочки.
 * @return `EXIT_SUCCESS` в случае успеха,
	код ошибки в случае синтаксической ошибки.
 */
int	parse_tokens(t_list *tokens, t_ast **syntax_tree, t_shell *shell)
{
	char	*invalid_token;
	int		parse_result;

	*syntax_tree = parse_logic(&tokens, shell);
	if (tokens)
	{
		invalid_token = get_value(tokens);
		record_synt_err(invalid_token, shell);
	}
	if (shell->syntax_error)
	{
		parse_result = display_synt_err(shell);
		return (parse_result);
	}
	return (EXIT_SUCCESS);
}