#include "minishell.h"

// нужно добавить обработку heredoc!

/**
 * resolve_ast_content
 * @brief Разрешает содержимое AST узла,
	выполняя замену переменных и аргументов команды.
 *
 * Эта функция обрабатывает два типа узлов AST: команды и редиректы.
 * Для узлов команд она выполняет подстановку аргументов,
	заменяя переменные на их значения.
 * Для узлов редиректов (кроме heredoc) она разрешает имя файла,
	выполняя аналогичную замену переменных.
 *
 * Подробное описание:

	* 1. Функция начинает с инициализации списка для хранения разрешенных аргументов.
 * 2. Если тип узла - команда (CMD):
 *    - Извлекает аргументы команды и обрабатывает каждый из них,
	выполняя подстановку переменных.
 *    - Заменяет исходные аргументы команды на разрешенные аргументы.
 * 3. Если тип узла - редирект (REDIR) и это не heredoc:
 *    - Извлекает имя файла и выполняет подстановку переменных.
 *    - Проверяет, что после подстановки остался один аргумент и он не пустой.
 *    - Если аргумент корректен, заменяет исходное имя файла на разрешенное.
 *    - В случае некорректного аргумента выводит сообщение об ошибке.
 *
 * @param node Узел AST для обработки.

	* @param shell Структура оболочки для управления памятью и синтаксическими ошибками.
 * @return Обработанный узел AST.
 */
t_ast	*resolve_ast_content(t_ast *node, t_shell *shell)
{
	t_list	*args_to_resolve;
	char	**cmd_args;
	char	*filename;

	args_to_resolve = NULL;
	if (node->node_type == CMD)
	{
		cmd_args = node->node_content.cmd.cmd_args;
		while (*cmd_args)
			resolve_arg(*cmd_args++, &args_to_resolve, shell);
		node->node_content.cmd.cmd_args = create_string_array(&args_to_resolve,
				shell);
	}
	else if (node->node_type == REDIR
		&& node->node_content.redir.redir_type != T_HDOC)
	{
		filename = node->node_content.redir.filename;
		resolve_arg(filename, &args_to_resolve, shell);
		if (ft_lstsize(args_to_resolve) != 1
			|| !ft_strcmp(args_to_resolve->content, ""))
			error_msg("redirection:", NULL, "invalid filename", shell);
		else
			node->node_content.redir.filename = args_to_resolve->content;
	}
	return (node);
}

/**
 * resolve_ev
 * @brief Обрабатывает переменные окружения в строке,
	включая специальную переменную "?".
 *
 * Эта функция принимает строку `input`,
	контекст подстановки `context` и структуру оболочки `shell`.
 * Она выполняет следующие шаги:
 * 1. Проверяет, является ли следующий символ после `$` символом `?`:
 *    - Если да, заменяет его на код возврата предыдущей команды.
 * 2. Если нет,
	и если символ в режиме вне кавычек и следующим символом является `"` или `'`,
	то возвращается без изменения.
 * 3. В противном случае, ищет значение переменной окружения,
	указанной после `$`.
 * 4. Если найдено значение переменной и режим вне кавычек,
	выполняет разделение слова.
 * 5. Если значение переменной найдено, добавляет его в буфер переменных.
 *
 * @param input Строка, содержащая переменные окружения для подстановки.

	* @param context Контекст подстановки для хранения информации о состоянии подстановки.
 * @param shell Структура оболочки,
	используемая для управления памятью и переменными окружения.
 */
void	resolve_ev(char *input, t_subst_context *context, t_shell *shell)
{
	char	*var_value;

	if (input[context->pos + 1] == '?')
	{
		context->pos++;
		var_value = manage_memory(ft_itoa(shell->prev_cmd_status),
				COMMAND_TRACK, shell);
	}
	else
	{
		if (context->quote_mode == OUTSIDE && (input[context->pos + 1] == '\"'
				|| input[context->pos + 1] == '\''))
			return ;
		var_value = extract_ev_value(input, context, shell);
		if (var_value && context->quote_mode == OUTSIDE)
			var_value = tokenize_words(context, var_value, shell);
	}
	if (var_value)
		expand_subst_buffer(var_value, context, shell);
}

/**
 * extract_ev_value
 * @brief Извлекает значение переменной окружения из входной строки.
 *
 * Эта функция принимает строку `input`,
	текущий контекст подстановки `context` и оболочку `shell`.
 * Она ищет и извлекает имя переменной окружения из строки,
	начиная с текущей позиции,
 * и возвращает значение этой переменной,
	если оно существует. Если имя переменной не найдено
 * или не является допустимым, функция возвращает `NULL`.
 *
 * 1. Проверяет,
	начинается ли переменная окружения с допустимого символа (буквы или '_').
 * 2. Если символ не является допустимым,
	добавляет текущий символ из `input` в буфер подстановки
 *    и возвращает `NULL`.
 * 3. Перемещается по строке,
	пока встречаются допустимые символы имени переменной окружения (буквы,
	цифры, '_').
 * 4. Извлекает имя переменной окружения из строки и выделяет для него память.
 * 5. Если имя переменной не найдено или недопустимо,
	добавляет текущий символ из `input` в буфер подстановки
 *    и возвращает `NULL`.

	* 6. Получает значение переменной окружения и обновляет текущую позицию в строке.
 * 7. Возвращает значение переменной окружения.
 *
 * @param input Строка, содержащая переменную окружения для извлечения.
 * @param context Текущий контекст подстановки,
	содержащий позицию и буфер подстановки.

	* @param shell Указатель на структуру оболочки для управления памятью и переменными окружения.
 * @return Значение переменной окружения или `NULL` в случае ошибки.
 */
char	*extract_ev_value(char *input, t_subst_context *context, t_shell *shell)
{
	int		cur_pos;
	char	*ev_name;
	char	*ev_value;

	cur_pos = context->pos + 1;
	if (!ft_isalpha(input[cur_pos]) && input[cur_pos] != '_')
	{
		context->subst_buffer[context->buf_pos++] = input[context->pos];
		context->subst_buffer[context->buf_pos] = '\0';
		return (NULL);
	}
	while (ft_isalnum(input[cur_pos]) || input[cur_pos] == '_')
		cur_pos++;
	ev_name = manage_memory(ft_substr(input, context->pos + 1, cur_pos
				- context->pos - 1), COMMAND_TRACK, shell);
	if (ev_name == NULL)
	{
		context->subst_buffer[context->buf_pos++] = input[context->pos];
		context->subst_buffer[context->buf_pos] = '\0';
		return (NULL);
	}
	ev_value = get_ev_value(get_ev(ev_name, shell->ev_list));
	context->pos += ft_strlen(ev_name);
	return (ev_value);
}

//-------quotes -------
/**
 * @brief Обрабатывает символы внутри кавычек в строке.
 *
 * Эта функция предназначена для обработки символов, когда они находятся
 * внутри одинарных или двойных кавычек в строке. В зависимости от типа кавычек
 * функция выполняет различные действия:
 *
 * 1. Если кавычки одинарные (quote_char == '\''):
 *    - Проверяет, является ли текущий символ закрывающей одинарной кавычкой.
 *    - Если да, и буфер пустой, устанавливает флаг is_empty_qts.
 *    - Переключает режим quote_mode на OUTSIDE.
 *    - В противном случае добавляет символ в буфер.
 *
 * 2. Если кавычки двойные (quote_char == '\"'):
 *    - Проверяет, является ли текущий символ знаком доллара для
 *      обработки переменных окружения.
 *    - Проверяет, является ли текущий символ закрывающей двойной кавычкой.
 *      Если да, и буфер пустой, устанавливает флаг is_empty_qts.
 *      Переключает режим quote_mode на OUTSIDE.
 *    - Обрабатывает экранированные символы внутри двойных кавычек.
 *    - В противном случае добавляет символ в буфер.
 *
 * @param quote_char Тип кавычек (одинарные или двойные).
 * @param arg Строка для обработки.
 * @param context Контекст обработки строки.
 * @param shell Указатель на структуру оболочки.
 */
void	process_quotes(char quote_char, char *arg, t_subst_context *context,
		t_shell *shell)
{
	if (quote_char == '\'')
	{
		if (arg[context->pos] == '\'')
		{
			context->is_empty_qts = (context->buf_pos == 0);
			context->quote_mode = OUTSIDE;
		}
		else
			context->subst_buffer[context->buf_pos++] = arg[context->pos];
		return ;
	}
	if (arg[context->pos] == '$')
		resolve_ev(arg, context, shell);
	else if (arg[context->pos] == '\"')
	{
		context->is_empty_qts = (context->buf_pos == 0);
		context->quote_mode = OUTSIDE;
	}
	else
	{
		if (arg[context->pos] == '\\' && (arg[context->pos + 1] == '\"'
				|| arg[context->pos + 1] == '\\'))
			context->pos++;
		context->subst_buffer[context->buf_pos++] = arg[context->pos];
	}
}
/**
 * @brief Обрабатывает символы вне кавычек в строке.
 *
 * Эта функция обрабатывает символы в строке, когда они находятся вне кавычек:
 * - Разделяет строку по пробелам.
 * - Обрабатывает переменные окружения, символы тильды и экранирование.
 * - Переключает режим кавычек.
 * - Добавляет символы в буфер.
 *
 * @param arg Строка для обработки.
 * @param context Контекст обработки строки.
 * @param shell Указатель на структуру оболочки.
 */
void	process_unquoted_chars(char *arg, t_subst_context *context,
		t_shell *shell)
{
	if (arg[context->pos] == ' ')
		insert_tkn(context, shell);
	else if (arg[context->pos] == '$')
		resolve_ev(arg, context, shell);
	else if (arg[context->pos] == '\'')
		context->quote_mode = SINGLE;
	else if (arg[context->pos] == '\"')
		context->quote_mode = DOUBLE;
	else if (arg[context->pos] == '\\')
		context->subst_buffer[context->buf_pos++] = arg[++context->pos];
	else if (arg[context->pos] == '~' && context->buf_pos == 0
		&& (arg[context->pos + 1] == '/' || arg[context->pos + 1] == '\0'))
	{
		if (get_ev_value(get_ev("HOME", shell->ev_list)))
			expand_subst_buffer(get_ev_value(get_ev("HOME", shell->ev_list)),
				context, shell);
		else if (shell->home_dir)
			expand_subst_buffer(shell->home_dir, context, shell);
	}
	else
	{
		if (arg[context->pos] == '*')
			locate_wildcards("*", context, shell);
		context->subst_buffer[context->buf_pos++] = arg[context->pos];
	}
}
/**
 * @brief Обрабатывает строку аргумента, выполняя замену переменных,
	кавычек и специальных символов.
 *
 * Эта функция принимает строку аргумента и выполняет следующие действия:
 * - Инициализирует контекст для отслеживания состояния обработки.
 * - Проходит по каждому символу строки,
	обрабатывая его в зависимости от текущего режима кавычек.
 * - Выполняет замену переменных окружения, обработку символа тильды (~),
	доллара ($), обратной косой черты (\) и других специальных символов.
 * - Добавляет обработанные токены в список аргументов.
 *
 * @param arg Строка аргумента для обработки.
 * @param arg_list Указатель на список,
	куда будут добавлены обработанные аргументы.
 * @param shell Указатель на структуру оболочки,
	содержащую информацию о состоянии среды выполнения.
 */
void	resolve_arg(char *arg, t_list **arg_list, t_shell *shell)
{
	t_subst_context	context;

	context.pos = 0;
	context.subst_buffer = calloc_tracked(ft_strlen(arg) + 1, sizeof(char),
			COMMAND_TRACK, shell);
	context.buf_pos = 0;
	context.capacity = ft_strlen(arg) + 1;
	context.quote_mode = OUTSIDE;
	context.is_empty_qts = false;
	context.tkn_list = arg_list;
	context.wildcard_list = NULL;
	while (arg[context.pos])
	{
		if (context.quote_mode == SINGLE)
			process_quotes('\'', arg, &context, shell);
		else if (context.quote_mode == DOUBLE)
			process_quotes('\"', arg, &context, shell);
		else if (context.quote_mode == OUTSIDE)
			process_unquoted_chars(arg, &context, shell);
		if (arg[context.pos])
			context.pos++;
	}
	if (context.quote_mode != OUTSIDE)
		exit_on_error("parsing", "missing closing quote", EXIT_FAILURE, shell);
	insert_tkn(&context, shell);
}

/**
 * @brief Обрабатывает имена файлов и добавляет их в список токенов.
 *
 * Функция завершает текущую строку в буфере, получает список файлов,
 * фильтрует его в соответствии с заданным шаблоном и добавляет
 * отфильтрованные файлы в список токенов.
 *
 * Шаги выполнения:
 * 1. Завершает текущую строку в буфере.
 * 2. Получает список файлов, соответствующих пути.
 * 3. Фильтрует список файлов в соответствии с заданным шаблоном.
 * 4. Добавляет отфильтрованные файлы в список токенов.
 * 5. Обнуляет буфер после обработки.
 *
 * @param ctx Указатель на контекст замены.
 * @param shell Указатель на структуру оболочки для ������правления памятью.
 */
void	process_filename(t_subst_context *ctx, t_shell *shell)
{
	t_list	*file_lst;

	ctx->subst_buffer[ctx->buf_pos] = '\0';
	file_lst = clean_file_list(retrive_files(ctx, shell), ctx);
	while (file_lst)
	{
		lstadd_back_tracked(file_lst->content, ctx->tkn_list, COMMAND_TRACK,
			shell);
		file_lst = file_lst->next;
	}
	ctx->buf_pos = 0;
	ctx->subst_buffer[ctx->buf_pos] = '\0';
}

/**
 * @brief Считывает записи каталога и добавляет их в список файлов.
 *
 * Эта функция открывает указанный каталог и считывает все его записи.
 * Для каждой записи, кроме текущего (".") и родительского ("..") каталогов,
 * она создает полное имя файла и добавляет его в указанный список файлов.
 * Если корневой путь не является ".", то полное имя файла будет включать
 * корневой путь. В противном случае, только имя файла будет добавлено.
 *
 * @param directory Указатель на открытый каталог.
 * @param root Путь к корневому каталогу.
 * @param file_list Указатель на список файлов, куда будут добавлены
 *                  найденные файлы.
 * @param shell Указатель на структуру shell для управления памятью.
 */
static void	read_directory_files(DIR *directory, char *root, t_list **file_list,
		t_shell *shell)
{
	struct dirent	*file_record;
	char			*file_name;

	file_record = read_directory(directory, shell);
	while (file_record != NULL)
	{
		if (ft_strcmp(file_record->d_name, ".")
			&& ft_strcmp(file_record->d_name, ".."))
		{
			if (ft_strcmp(root, "."))
				file_name = strjoin_tracked(root, file_record->d_name,
						COMMAND_TRACK, shell);
			else
				file_name = strdup_tracked(file_record->d_name, COMMAND_TRACK,
						shell);
			add_file_to_list(file_name, file_list, shell);
		}
		file_record = read_directory(directory, shell);
	}
}

/**
 * @brief Извлекает список файлов из указанного каталога.
 *
 * Эта функция извлекает корневой путь из буфера замены контекста,
 * открывает соответствующий каталог, считывает его содержимое
 * с помощью функции read_directory_files и возвращает список найденных
 * файлов. Если каталог не может быть открыт или он пуст, функция
 * возвращает NULL.
 *
 * @param context Указатель на контекст замены для получения корневого пути.
 * @param shell Указатель на структуру shell для управления памятью.
 * @return Указатель на список файлов или NULL, если каталог пуст или
 *         не может быть открыт.
 */
t_list	*retrive_files(t_subst_context *context, t_shell *shell)
{
	DIR		*directory;
	char	*root;
	char	*slash_pos;
	t_list	*file_list;
	int		len;

	file_list = NULL;
	slash_pos = ft_strrchr(context->subst_buffer, '/');
	if (slash_pos == NULL)
		root = strdup_tracked(".", COMMAND_TRACK, shell);
	else
	{
		len = slash_pos - context->subst_buffer + 1;
		root = calloc_tracked(len + 1, sizeof(char), COMMAND_TRACK, shell);
		ft_strlcpy(root, context->subst_buffer, len + 1);
	}
	directory = open_directory(root, shell);
	if (directory != NULL)
	{
		read_directory_files(directory, root, &file_list, shell);
		close_directory(directory, shell);
	}
	return (file_list);
}

/**
 * Вставляет новый узел в начало списка.
 *
 * @param file_list Указатель на указатель на первый узел списка.
 * @param new_node Указатель на новый узел, который нужно вставить.
 */
static void	insert_at_beginning(t_list **file_list, t_list *new_node)
{
	if (*file_list != NULL)
		(*file_list)->prev = new_node;
	new_node->next = *file_list;
	*file_list = new_node;
}

/**
 * Вставляет новый узел в середину списка после указанного узла.
 *
 * @param cur_node Указатель на узел, после которого нужно вставить новый узел.
 * @param new_node Указатель на новый узел, который нужно вставить.
 */
static void	insert_in_middle(t_list *cur_node, t_list *new_node)
{
	if (cur_node->next != NULL)
		cur_node->next->prev = new_node;
	new_node->next = cur_node->next;
	cur_node->next = new_node;
	new_node->prev = cur_node;
}

/** ФУНКЦИЯ - УРОДКА. ПЕРЕДЕЛАТЬ!
 * @brief Добавляет новый файл в связанный список в алфавитном порядке.
 *
 * Эта функция создает новый узел списка, содержащий имя файла, и вставляет
 * его в связанный список в правильное место, чтобы сохранить алфавитный
 * порядок. Если список пуст, новый узел становится первым элементом.
 * Если имя файла лексикографически меньше имени первого узла, новый узел
 * вставляется в начало списка. В противном случае, функция находит
 * подходящее место в списке и вставляет новый узел.
 *
 * @param filename Имя файла, который нужно добавить в список.
 * @param file_list Указатель на указатель на первый узел списка.
 * @param shell Указатель на структуру оболочки,
	используемую для управления памятью.
 */
void	add_file_to_list(char *filename, t_list **file_list, t_shell *shell)
{
	t_list	*new_file;
	t_list	*cur_node;

	new_file = ft_lstnew(strdup_tracked(filename, COMMAND_TRACK, shell));
	alloc_check(new_file, filename, shell);
	track_memory(new_file, COMMAND_TRACK, shell);
	if (*file_list == NULL || ft_strcmp(filename, (*file_list)->content) < 0)
	{
		insert_at_beginning(file_list, new_file);
		return ;
	}
	cur_node = *file_list;
	while (cur_node->next != NULL)
	{
		if (ft_strcmp(cur_node->next->content, filename) >= 0)
			break ;
		cur_node = cur_node->next;
	}
	insert_in_middle(cur_node, new_file);
}

/**
 * @brief Очищает список файлов, удаляя записи, которые не соответствуют
 * заданному шаблону или считаются скрытыми файлами.
 *
 * Эта функция проходит по предоставленному списку файлов и фильтрует их
 * на основе соответствия шаблону из буфера подстановки (context->subst_buffer)
 * и проверки на скрытые файлы. Файлы, которые не соответствуют шаблону или
 * являются скрытыми, удаляются из списка.
 *
 * @param file_list Список файлов для фильтрации.
 * @param context Контекст подстановки, содержащий буфер шаблона.
 * @return Очищенный список файлов, содержащий только соответствующие
 * шаблону и не скрытые файлы.
 */
t_list	*clean_file_list(t_list *file_list, t_subst_context *context)
{
	t_list	*cur_file;
	t_list	*previous;
	t_list	*next;
	char	*filename;

	cur_file = file_list;
	previous = NULL;
	while (cur_file)
	{
		filename = (char *)cur_file->content;
		next = cur_file->next;
		if (wildcard_check(filename, context->subst_buffer, 0, context)
			&& is_file_visible(filename, context))
			previous = cur_file;
		else
		{
			if (previous)
				previous->next = next;
			else
				file_list = next;
			del_node(&cur_file, NULL, NULL, false);
		}
		cur_file = next;
	}
	return (file_list);
}
