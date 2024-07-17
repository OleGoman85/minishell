
#include "minishell.h"


/**
 * @brief Обрабатывает символ '*' (wildcard) в шаблоне.
 *

	* Эта функция продвигает указатель в шаблоне и проверяет все возможные подстроки
 * строки файла для совпадения с оставшейся частью шаблона. Если символ '*'
 * является последним в шаблоне, функция возвращает true. В противном случае,
 * она рекурсивно вызывает `wildcard_check` для каждой возможной подстроки.
 *
 * @param file_ptr Указатель на указатель текущей позиции в строке файла.
 * @param mask_ptr Указатель на указатель текущей позиции в строке шаблона.
 * @param pos Указатель на текущую позицию в шаблоне.
 * @param context Указатель на структуру контекста подстановки,
 *                содержащую дополнительную информацию для обработки wildcard.
 * @return true, если найдено соответствие, иначе false.
 */
bool	process_asterisk(const char **file_ptr, const char **mask_ptr, int *pos,
		t_subst_context *context)
{
	(*mask_ptr)++;
	(*pos)++;
	if (!**mask_ptr)
		return (true);
	while (**file_ptr)
	{
		if (wildcard_check(*file_ptr, *mask_ptr, *pos, context))
			return (true);
		(*file_ptr)++;
	}
	return (false);
}

/**

* @brief Проверяет соответствие строки файлу с учетом маски подстановочных символов.
 *
 * Эта функция проверяет, соответствует ли строка имени файла заданной маске,

	* содержащей подстановочные символы ('*'). Маска может содержать один или несколько
 * подстановочных символов,
	которые соответствуют нулю или более символам в имени файла.
 *
 * Функция выполняет итерацию по строкам файла и маски,
	сравнивая символы по одному,
 * и обрабатывает подстановочные символы '*' при их обнаружении.
 *
 * @param file Указатель на строку имени файла для проверки.
 * @param wildcard_mask Указатель на строку маски,
	содержащую подстановочные символы.
 * @param pos Текущая позиция в строке маски.
 * @param context Контекст подстановки,
	содержащий список позиций подстановочных символов.
 * @return true, если строка файла соответствует маске,
	и false в противном случае.
 */
bool	wildcard_check(const char *file, const char *wildcard_mask, int pos,
		t_subst_context *context)
{
	while (*file && *wildcard_mask)
	{
		if (*wildcard_mask == '*' && in_wildcard_list(pos, context))
		{
			if (process_asterisk(&file, &wildcard_mask, &pos, context))
				continue ;
			else
				return (false);
		}
		else if (*wildcard_mask++ != *file++)
			return (false);
		pos++;
	}
	while (*wildcard_mask == '*' && in_wildcard_list(pos, context))
	{
		wildcard_mask++;
		pos++;
	}
	if (*wildcard_mask == '\0' && *file == '\0')
		return (true);
	else
		return (false);
}

/**
 * @brief Проверяет,
	есть ли текущая позиция в списке активных подстановочных символов.
 *
 * Эта функция проходит по списку позиций wildcard в контексте подстановки и
 * проверяет, есть ли текущая позиция среди них.
 *
 * @param pos Текущая позиция для проверки.
 * @param context Указатель на структуру контекста подстановки,
 *                содержащую список позиций wildcard.
 * @return true, если текущая позиция является активным wildcard, иначе false.
 */
bool	in_wildcard_list(int pos, t_subst_context *context)
{
	t_list *node;
	int *current_pos;
	bool is_match;

	node = context->wildcard_list;
	while (node != NULL)
	{
		current_pos = (int *)(node->content);
		is_match = (*current_pos == pos);
		if (is_match)
			return (true);
		node = node->next;
	}
	return (false);
}

/**

	* @brief Находит позиции символов '*' в строке и сохраняет их в контексте подстановки.
 *
 * Функция проходит по строке, находит все символы '*' и сохраняет их позиции

	* относительно начала строки в списке подстановочных символов в контексте подстановки.
 * Позиции сохраняются только в том случае,
	если текущий режим кавычек является OUTSIDE.
 *
 * @param str Строка, в которой осуществляется поиск символов '*'.
 * @param context Указатель на контекст подстановки,
	содержащий информацию о текущем режиме кавычек и буфере.
 * @param shell Указатель на структуру оболочки для управления памятью.
 */
void	locate_wildcards(char *str, t_subst_context *context, t_shell *shell)
{
	int *wildcard_ptr;
	int i;

	if (context->quote_mode == OUTSIDE)
	{
		wildcard_ptr = NULL;
		i = 0;
		while (str[i] != '\0')
		{
			if (str[i] == '*')
			{
				wildcard_ptr = calloc_tracked(1, sizeof(int), COMMAND_TRACK,
						shell);
				*wildcard_ptr = context->buf_pos + i;
				lstadd_back_tracked(wildcard_ptr, &context->wildcard_list,
					COMMAND_TRACK, shell);
			}
			i++;
		}
	}
}

/**
 * @brief Проверяет, видим ли файл, основываясь на его имени и маске.
 *
 * Эта функция проверяет, является ли файл скрытым (начинается с '.')
 * и разрешает ли маска отображение скрытых файлов. Если файл скрыт,
 * а маска не разрешает отображение скрытых файлов, функция возвращает true.
 *
 * @param filepath Путь к файлу для проверки.
 * @param context Указатель на структуру контекста подстановки,
	содержащую маску.
 * @return (true, если файл скрыт,
	а маска не разрешает отображение скрытых файлов);
 *         false в противном случае.
 */
bool	is_file_visible(const char *filepath, t_subst_context *context)
{
	const char *mask_segment;
	const char *name_segment;
	bool file_is_hidden;
	bool mask_allows_hidden;

	mask_segment = ft_strrchr(context->subst_buffer, '/');
	name_segment = ft_strrchr(filepath, '/');
	if (!mask_segment)
		mask_segment++;
	else
		mask_segment = context->subst_buffer;
	if (name_segment)
		name_segment++;
	else
		name_segment = filepath;
	file_is_hidden = (name_segment[0] == '.');
	mask_allows_hidden = (mask_segment[0] != '.');
	if (file_is_hidden && mask_allows_hidden)
		return (true);
	return (false);
}

/**
 * @brief Вставляет текущий буфер как новый токен в список токенов контекста.
 *
 * Эта функция проверяет текущий буфер кон��екста и добавляет его содержимое
 * в список токенов. Если буфер пуст и флаг is_empty_qts установлен,
	добавляется
 * пустая строка. Если буфер содержит данные, они копируются в новый токен и
 * добавляются в список. Если в контексте есть список подстановочных знаков
 * (wildcards), вызывается функция обработки имен файлов.
 *
 * @param context Указатель на контекст замены,
	содержащий информацию о буфере и токенах.
 * @param shell Указатель на структуру оболочки для управления памятью.
 * @return Всегда возвращает NULL.
 */
void	*insert_tkn(t_subst_context *context, t_shell *shell)
{
	char *new_tkn;

	new_tkn = NULL;
	if (context->buf_pos == 0 && context->is_empty_qts)
	{
		new_tkn = strdup_tracked("", COMMAND_TRACK, shell);
		context->is_empty_qts = false;
	}
	else if (context->buf_pos != 0)
	{
		context->subst_buffer[context->buf_pos] = '\0';
		new_tkn = strdup_tracked(context->subst_buffer, COMMAND_TRACK, shell);
		context->buf_pos = 0;
	}
	else
		return (NULL);
	if (new_tkn != NULL)
		lstadd_back_tracked(new_tkn, context->tkn_list, COMMAND_TRACK, shell);
	if (context->wildcard_list)
		process_filename(context, shell);
	return (NULL);
}

/**
 * @brief Добавляет значение переменной в буфер замены контекста.
 *
 * Эта функция увеличивает ёмкость буфера замены в контексте,
	чтобы вместить новое значение,

	* и добавляет его в текущий буфер. Также функция обновляет список подстановочных знаков.
 *
 * @param value Значение переменной, которое нужно добавить в буфер.
 * @param context Указатель на контекст замены,
	содержащий информацию о буфере и подстановках.
 * @param shell Указатель на структуру оболочки для управления памятью.
 */
void	expand_subst_buffer(char *var_value, t_subst_context *context,
		t_shell *shell)
{
	char *new_buffer;
	size_t value_len;
	size_t new_capacity;

	value_len = ft_strlen(var_value);
	new_capacity = context->capacity + value_len;
	locate_wildcards(var_value, context, shell);
	new_buffer = calloc_tracked(new_capacity, sizeof(char), COMMAND_TRACK,
			shell);
	ft_strlcpy(new_buffer, context->subst_buffer, context->buf_pos + 1);
	ft_strlcat(new_buffer, var_value, context->buf_pos + value_len + 1);
	context->subst_buffer = new_buffer;
	context->buf_pos += value_len;
	context->capacity = new_capacity;
}

/**
 * @brief Преобразует связанный список в массив строк.
 *
 * Эта функция принимает связанный список и создает новый массив строк,

	* содержащий элементы списка. Завершающий элемент массива устанавливается в NULL.
 *
 * @param list Указатель на указатель на первый элемент связанного списка.
 * @param shell Указатель на структуру оболочки для управления памятью.
 * @return Указатель на массив строк или NULL,
	если выделение памяти не удалось.
 */
char	**create_string_array(t_list **list, t_shell *shell)
{
	t_list *node;
	char **array;
	int list_size;
	int index;

	list_size = ft_lstsize(*list);
	array = (char **)calloc_tracked(list_size + 1, sizeof(char *),
			COMMAND_TRACK, shell);
	if (!array)
		return (NULL);
	node = *list;
	index = 0;
	while (node != NULL)
	{
		array[index] = (char *)node->content;
		node = node->next;
		index++;
	}
	array[index] = NULL;
	return (array);
}

/**
 * @brief Обрабатывает и добавляет токены в контекст замены.
 *
 * Эта функция разбирает строку на токены, обрабатывает каждый токен и 
 * добавляет его в список токенов в контексте замены. Если токен начинается
 * с пробела, он добавляется отдельно. Если токен не заканчивается пробелом,
 * он добавляется в конец списка.
 *
 * @param tokens Указатель на массив токенов.
 * @param context Указатель на контекст замены.
 * @param shell Указатель на структуру оболочки для управления памятью.
 * @param str Исходная строка, которая была разбита на токены.
 * @return Указатель на последний токен, если он не заканчивается пробелом,
 *         или NULL.
 */
char	*handle_tokens(char **tokens, t_subst_context *context, t_shell *shell,
		char *str)
{
	bool starts_with_space = (str[0] == ' ');
	bool ends_with_space = (str[ft_strlen(str) - 1] == ' ');

	if (*tokens)
	{
		if (!starts_with_space)
		{
			expand_subst_buffer(*tokens++, context, shell);
			if (!*tokens)
				return (insert_tkn(context, shell));
		}
		else
			insert_tkn(context, shell);
		while (*tokens)
		{
			insert_tkn(context, shell);
			if (!*(tokens + 1) && !ends_with_space)
				return (*tokens);
			lstadd_back_tracked(*tokens++, context->tkn_list, COMMAND_TRACK,
				shell);
		}
	}
	else
		insert_tkn(context, shell);
	return (NULL);
}


/**
 * @brief Разбивает строку на токены и обрабатывает их.
 *
 * Эта функция разбивает входную строку на токены, используя пробелы как 
 * разделители, и вызывает handle_tokens для обработки каждого токена. 
 * Если строка пуста, она возвращается как есть. Если строка содержит только 
 * один токен, она также возвращается без изменений.
 *
 * @param context Указатель на контекст замены.
 * @param str Исходная строка, которую нужно разбить на токены.
 * @param shell Указатель на структуру оболочки для управления памятью.
 * @return Указатель на последний токен, если он не заканчивается пробелом,
 *         или NULL.
 */
char	*tokenize_words(t_subst_context *context, char *str, t_shell *shell)
{
	char **tokens;
	char *result;

	if (strlen(str) == 0)
		return (str);

	tokens = split_tracked(str, ' ', COMMAND_TRACK, shell);
	if (!tokens || !*tokens)
		return (insert_tkn(context, shell));
	if (ft_strcmp(str, tokens[0]) == 0)
		return (str);
	result = handle_tokens(tokens, context, shell, str);
	return (result);
}
