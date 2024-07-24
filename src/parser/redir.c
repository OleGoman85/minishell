
#include "minishell.h"

/** link_redir_to_cmd
 * @brief Присоединяет команду к последовательности редиректов.
 *

	* Функция `link_redir_to_cmd` принимает последовательность редиректов `redir_sequence`
 * и команду `cmd`. Она находит последний редирект в последовательности и
 * устанавливает его командой `cmd`.
 *
 * @param redir_sequence Указатель на AST узел,
	представляющий последовательность редиректов.
 * @param cmd Указатель на AST узел команды.
 * @return Указатель на AST узел обновленной последовательности редиректов.
 */
t_ast	*link_redir_to_cmd(t_ast *redir_sequence, t_ast *cmd)
{
	t_ast	*redir_tail;

	redir_tail = find_redir_tail(redir_sequence);
	redir_tail->node_content.redir.command = cmd;
	return (redir_sequence);
}

/** assemble_cmd
 * @brief Собирает команду с префиксом и суффиксом редиректов.
 *
 * Функция `assemble_cmd` принимает префикс редиректов `redir_prefix`,
 * суффикс редиректов `redir_suffix` и команду `cmd`. Она соединяет все части
 * так, чтобы командная цепочка была последовательной и корректной.
 *
 * @param redir_prefix Указатель на AST узел, представляющий префикс редиректов.
 * @param redir_suffix Указатель на AST узел, представляющий суффикс редиректов.
 * @param cmd Указатель на AST узел команды.
 * @return Указатель на AST узел,
	представляющий собранную команду с редиректами.
 */
t_ast	*assemble_cmd(t_ast *redir_prefix, t_ast *redir_suffix, t_ast *cmd)
{
	if (redir_prefix)
	{
		if (redir_suffix)
		{
			redir_prefix = link_redir_to_cmd(redir_prefix, redir_suffix);
			redir_suffix = link_redir_to_cmd(redir_suffix, cmd);
			return (redir_prefix);
		}
		return (link_redir_to_cmd(redir_prefix, cmd));
	}
	if (redir_suffix)
		return (link_redir_to_cmd(redir_suffix, cmd));
	return (cmd);
}

/**
 * find_redir_tail
 * @brief Находит и возвращает последний узел редиректа в цепочке редиректов.
 *
 * Функция принимает указатель на первый узел цепочки редиректов (redir_head)
 * и проходит по цепочке,
	пока не достигнет последнего узла редиректа. Узлы редиректа
 * определяются по типу `REDIR`, а конец цепочки определяется по отсутствию
 * команды редиректа или по наличию команды, которая не является редиректом.
 *
 * @param redir_head Указатель на первый узел цепочки редиректов.

	* @return Указатель на последний узел редиректа в цепочке или на последний узел,
 *         который не является редиректом, если цепочка заканчивается командой.
 */
t_ast	*find_redir_tail(t_ast *redir_head)
{
	t_ast	*current_node;

	current_node = redir_head;
	while (current_node != NULL)
	{
		if (current_node->node_type != REDIR)
			break ;
		if (current_node->node_content.redir.command == NULL)
			break ;
		if (current_node->node_content.redir.command->node_type != REDIR)
			break ;
		current_node = current_node->node_content.redir.command;
	}
	return (current_node);
}

/**
 * @brief Обрабатывает редиректы и создает AST (Abstract Syntax Tree) узел команды с редиректами.
 *
 * Функция `process_redir` выполняет следующие шаги:
 * 1. Извлекает редиректы, предшествующие команде (`initial_redir`).
 * 2. Если обнаружена синтаксическая ошибка, возвращает `NULL`.
 * 3. Парсит команду, обернутую в скобки, с использованием `parse_brace`.
 * 4. Если команда не найдена или обнаружена синтаксическая ошибка, возвращает `initial_redir`.
 * 5. Извлекает редиректы, следующие за командой (`end_redir`).
 * 6. Если обнаружена синтаксическая ошибка, возвращает `initial_redir`.
 * 7. Если есть последующие редиректы, объединяет их с командой и возвращает.
 * 8. Если есть только начальные редиректы, объединяет их с командой и возвращает.
 * 9. Если редиректов нет, возвращает узел команды.
 *
 * @param tokens Указатель на список токенов.
 * @param shell Структура оболочки.
 * @return Указатель на AST узел команды с редиректами или `NULL` при обнаружении синтаксической ошибки.
 */
t_ast	*process_redir(t_list **tokens, t_shell *shell)
{
	t_ast	*initial_redir;
	t_ast	*cmd_node;
	t_ast	*end_redir;

	initial_redir = extract_redirections(tokens, NULL, shell);
	if (shell->syntax_error)
		return (NULL);
	cmd_node = parse_brace(tokens, shell);
	if (cmd_node == NULL || shell->syntax_error)
	{
		if (cmd_node == NULL)
			return (initial_redir);
		return (NULL);
	}
	end_redir = extract_redirections(tokens, cmd_node, shell);
	if (shell->syntax_error)
		return (initial_redir);
	if (end_redir != NULL)
		return (assemble_cmd(initial_redir, end_redir, cmd_node));
	if (initial_redir != NULL)
		return (assemble_cmd(initial_redir, NULL, cmd_node));
	return (cmd_node);
}


/** add_cmd_arg
 * @brief Добавляет аргумент команды в AST узел команды,
	если текущий токен является текстом.
 *
 * Функция `add_cmd_arg` выполняет следующие шаги:
 * 1. Проверяет, что переданные указатели `tokens`,
	`cmd_node` и `shell` не равны NULL.
 * 2. Получает тип текущего токена.
 * 3. Если тип токена не является текстом (T_TEXT), возвращает `false`.

	* 4. Получает значение текущего токена и добавляет его в массив аргументов команды `cmd_args` узла команды `cmd_node`.
 * 5. Продвигает указатель `tokens` на следующий токен.
 * 6. Возвращает `true` при успешном добавлении аргумента команды.
 *
 * @param tokens Указатель на список токенов.
 * @param cmd_node Указатель на AST узел команды.
 * @param shell Структура оболочки.
 * @return `true`, если аргумент команды был успешно добавлен, иначе `false`.
 */
bool	add_cmd_arg(t_list **tokens, t_ast *cmd_node, t_shell *shell)
{
	t_tkn_type	tkn_type;
	char		*token_value;

	if (tokens == NULL || *tokens == NULL || cmd_node == NULL)
		return (false);
	tkn_type = get_type(*tokens);
	if (tkn_type != T_TEXT)
		return (false);
	token_value = get_value(*tokens);
	append_str(&cmd_node->node_content.cmd.cmd_args, token_value, shell);
	*tokens = (*tokens)->next;
	return (true);
}

/**
 * @brief Проверяет,
	является ли текущий токен допустимым редиректом или текстом,
	который сопровождается командой.
 *

	* Функция `is_valid_redir` принимает указатель на список токенов и командный узел `cmd_node`.
 * Она выполняет следующие шаги:
 * 1. Если `tokens` или текущий токен `NULL`, возвращает `false`.
 * 2. Получает тип текущего токена.
 * 3. Если токен типа `T_TEXT` и присутствует командный узел `cmd_node`,
	возвращает `true`.
 * 4. Если токен типа `T_APPEND`, `T_HDOC`, `T_INPUT` или `T_OUTPUT`,
	возвращает `true`.
 * 5. В остальных случаях возвращает `false`.
 *
 * @param tokens Указатель на список токенов.
 * @param cmd_node Узел команды.
 * @return `true`, если токен допустим, `false` в противном случае.
 */
bool	is_valid_redir(t_list **tokens, t_ast *cmd_node)
{
	t_tkn_type	type;

	if (tokens == NULL || *tokens == NULL)
		return (false);
	type = get_type(*tokens);
	if (cmd_node && type == T_TEXT)
		return (true);
	if (type == T_APPEND || type == T_HDOC || type == T_INPUT
		|| type == T_OUTPUT)
		return (true);
	return (false);
}

/**
 * @brief Добавляет новый узел редиректа в цепочку редиректов.
 *

	* Функция `append_redir_node` принимает два узла AST: `redir_chain` и `new_redir`.
 * Если `redir_chain` равен NULL, возвращает `new_redir`.

	* Иначе проходит по цепочке редиректов `redir_chain` до конца и добавляет `new_redir`.
 *
 * @param redir_chain Начало цепочки узлов редиректов.
 * @param new_redir Новый узел редиректа для добавления.
 * @return Начало цепочки узлов редиректов с добавленным узлом.
 */
t_ast	*append_redir_node(t_ast *redir_chain, t_ast *new_redir)
{
	t_ast	*current_redir;

	if (redir_chain == NULL)
		return (new_redir);
	current_redir = redir_chain;
	while (current_redir->node_content.redir.command)
		current_redir = current_redir->node_content.redir.command;
	current_redir->node_content.redir.command = new_redir;
	return (redir_chain);
}

/**
 * @brief Извлекает и обрабатывает редиректы из списка токенов.
 *
 * Функция `extract_redirections` проходит по списку токенов `tokens`,
	извлекает узлы редиректов,
 * добавляет их в цепочку редиректов и возвращает эту цепочку.
 * Если токен является аргументом команды, он добавляется в `cmd_node`.
 * Если найден редирект, создается новый узел редиректа,
	который добавляется в цепочку.
 *
 * @param tokens Указатель на список токенов.
 * @param cmd_node Узел команды, к которому будут добавляться аргументы.
 * @param shell Структура оболочки.
 * @return Начало цепочки узлов редиректов или NULL в случае ошибки.
 */
t_ast	*extract_redirections(t_list **tokens, t_ast *cmd_node, t_shell *shell)
{
	t_ast	*redir_chain;
	t_ast	*new_redir;

	redir_chain = NULL;
	while (is_valid_redir(tokens, cmd_node))
	{
		if (add_cmd_arg(tokens, cmd_node, shell))
			continue ;
		new_redir = build_node_redir(get_type(*tokens), (*tokens)->next, NULL,
				shell);
		if (new_redir == NULL)
			return (NULL);
		redir_chain = append_redir_node(redir_chain, new_redir);
		*tokens = (*tokens)->next->next;
	}
	return (redir_chain);
}
