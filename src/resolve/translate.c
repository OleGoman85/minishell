/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:03:03 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 09:58:36 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Resolves the content of an AST node by performing variable and 
 * command argument substitutions.
 *
 * This function processes two types of AST nodes: commands and redirects.
 * For command nodes, it performs substitution on arguments, replacing 
 * variables with their values.
 * For redirect nodes (excluding heredoc), it resolves the filename by 
 * performing similar variable substitution.
 *
 * Detailed description:
 * 
 * 1. The function starts by initializing a list to hold resolved arguments.
 * 2. If the node type is CMD:
 *    - Extracts the command arguments and processes each of them, performing 
 * variable substitution.
 *    - Replaces the original command arguments with the resolved arguments.
 * 3. If the node type is REDIR and it's not a heredoc:
 *    - Extracts the filename and performs variable substitution.
 *    - Checks that there is exactly one argument left after substitution and
 *  that it is not empty.
 *    - If the argument is valid, replaces the original filename with the
 *  resolved one.
 *    - If the argument is invalid, outputs an error message.
 *
 * @param node The AST node to process.
 * @param shell The shell structure for memory management and syntax error
 *  handling.
 * @return The processed AST node.
 */
t_ast	*resolve_ast_content(t_ast *node, t_shell *shell)
{
	t_list	*args_to_resolve;
	char	**cmd_args;
	char	*filename;

	args_to_resolve = NULL;
	if (node->node_type == CMD)
	{
		cmd_args = node->u_node_cont.cmd.cmd_args;
		while (*cmd_args)
			resolve_arg(*cmd_args++, &args_to_resolve, shell);
		node->u_node_cont.cmd.cmd_args = create_string_array(&args_to_resolve,
				shell);
	}
	else if (node->node_type == REDIR
		&& node->u_node_cont.redir.redir_type != T_HDOC)
	{
		filename = node->u_node_cont.redir.filename;
		resolve_arg(filename, &args_to_resolve, shell);
		if (ft_lstsize(args_to_resolve) != 1
			|| !ft_strcmp(args_to_resolve->content, ""))
			error_msg("redirection:", NULL, "invalid filename", shell);
		else
			node->u_node_cont.redir.filename = args_to_resolve->content;
	}
	return (node);
}

/**
 * @brief Processes environment variables in a string, including the special 
 * variable "?".
 *
 * This function takes the `input` string, substitution context `context`, 
 * and shell structure `shell`.
 * It performs the following steps:
 * 1. Checks if the character following `$` is `?`:
 *    - If so, replaces it with the return code of the previous command.
 * 2. If not, and if the character is outside of quotes and the next character
 * is `"` or `'`, returns without modification.
 * 3. Otherwise, searches for the value of the environment variable 
 * specified after `$`.
 * 4. If a variable value is found and outside quotes, performs word 
 * tokenization.
 * 5. If a variable value is found, adds it to the variable buffer.
 *
 * @param input The string containing environment variables for substitution.
 * @param context The substitution context holding the substitution state 
 * information.
 * @param shell The shell structure used for memory management and 
 * environment variables.
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
 * @brief Extracts the value of an environment variable from the input string.
 *
 * This function takes the `input` string, the current substitution context 
 * `context`, and the shell `shell`.
 * It searches and extracts the environment variable name from the string 
 * starting from the current position,
 * and returns the value of that variable if it exists. If the variable name 
 * is not found or invalid,
 * the function returns `NULL`.
 *
 * 1. Checks if the environment variable starts with a valid character 
 * (letter or `_`).
 * 2. If the character is not valid, adds the current character from 
 * `input` to the substitution buffer
 *    and returns `NULL`.
 * 3. Moves through the string while valid environment variable name 
 * characters (letters, digits, `_`) are found.
 * 4. Extracts the environment variable name from the string and allocates 
 * memory for it.
 * 5. If the variable name is not found or is invalid, adds the current 
 * character from `input` to the substitution buffer
 *    and returns `NULL`.
 * 6. Retrieves the value of the environment variable and updates the current
 *  position in the string.
 * 7. Returns the value of the environment variable.
 *
 * @param input The string containing the environment variable to extract.
 * @param context The current substitution context holding the position and
 *  substitution buffer.
 * @param shell Pointer to the shell structure for memory management and
 *  environment variables.
 * @return The value of the environment variable or `NULL` if an error occurs.
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

/**
 * @brief Processes characters inside quotes in the string.
 *
 * This function is designed to handle characters when they are inside single 
 * or double quotes in the string.
 * Depending on the type of quotes, the function performs different actions:
 *
 * 1. If the quotes are single (quote_char == '\''):
 *    - Checks if the current character is a closing single quote.
 *    - If yes, and the buffer is empty, sets the is_empty_qts flag.
 *    - Switches the quote_mode to OUTSIDE.
 *    - Otherwise, adds the character to the buffer.
 *
 * 2. If the quotes are double (quote_char == '\"'):
 *    - Checks if the current character is a dollar sign for environment 
 * variable processing.
 *    - Checks if the current character is a closing double quote. If yes,
 *  and the buffer is empty, sets the is_empty_qts flag.
 *      Switches the quote_mode to OUTSIDE.
 *    - Processes escaped characters inside double quotes.
 *    - Otherwise, adds the character to the buffer.
 *
 * @param quote_char The type of quotes (single or double).
 * @param arg The string to process.
 * @param context The context of string processing.
 * @param shell Pointer to the shell structure.
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
 * @brief Processes characters that are not enclosed in quotes.
 *
 * This function handles specific characters within a string that are not 
 * surrounded by quotes. It processes:
 * - Space characters by inserting a token.
 * - Dollar signs (`$`) for variable resolution.
 * - Single quotes (`'`) to switch to single quote mode.
 * - Double quotes (`"`) to switch to double quote mode.
 * - Backslashes (`\`) for escaping the next character.
 * - Tilde (`~`) for home directory expansion.
 * - Asterisks (`*`) for wildcard matching.
 * - All other characters are added to the substitution buffer.
 *
 * @param arg The argument string to process.
 * @param context The substitution context holding the current state.
 * @param shell Pointer to the shell structure for memory management.
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
 * @brief Processes an argument string, performing variable substitution, 
 * quote handling, and special character processing.
 *
 * This function initializes a context for tracking the substitution state, 
 * processes each character of the input string
 * based on the current quote mode, performs environment variable substitution,
 *  tilde expansion, and handles special characters.
 * Processed tokens are added to the argument list.
 *
 * @param arg The argument string to process.
 * @param arg_list Pointer to the list where processed arguments will be added.
 * @param shell Pointer to the shell structure containing runtime environment 
 * information.
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
 * @brief Processes file names and adds them to the token list.
 *
 * This function completes the current string in the buffer, retrieves a list 
 * of files matching the path,
 * filters the files based on the given pattern, and adds the filtered files 
 * to the token list.
 * It then resets the buffer after processing.
 *
 * @param ctx Pointer to the substitution context.
 * @param shell Pointer to the shell structure for memory management.
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
 * @brief Reads directory entries and adds them to the file list.
 *
 * This function opens the specified directory and reads its entries. For each 
 * entry, except for the current (".") 
 * and parent ("..") directories, it creates a full file name and adds it to 
 * the provided file list. If the root path 
 * is not ".", the full file name includes the root path; otherwise, only the 
 * file name is added.
 *
 * @param directory Pointer to the opened directory.
 * @param root Path to the root directory.
 * @param file_list Pointer to the file list where found files will be added.
 * @param shell Pointer to the shell structure for memory management.
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
 * @brief Retrieves a list of files from the specified directory.
 *
 * This function extracts the root path from the substitution buffer context, 
 * opens the corresponding directory,
 * reads its contents using the `read_directory_files` function, and returns
 *  the list of found files. If the directory 
 * cannot be opened or is empty, the function returns NULL.
 *
 * @param context Pointer to the substitution context for retrieving the root
 *  path.
 * @param shell Pointer to the shell structure for memory management.
 * 
 * @return Pointer to the list of files, or NULL if the directory is empty 
 * or cannot be opened.
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
 * @brief Inserts a new node at the beginning of the list.
 *
 * This function inserts a new node at the start of the list.
 *
 * @param file_list Pointer to the pointer to the first node of the list.
 * @param new_node Pointer to the new node to be inserted.
 */
static void	insert_at_beginning(t_list **file_list, t_list *new_node)
{
	if (*file_list != NULL)
		(*file_list)->prev = new_node;
	new_node->next = *file_list;
	*file_list = new_node;
}

/**
 * @brief Inserts a new node in the middle of the list after the 
 * specified node.
 *
 * This function inserts a new node after a given node in the list.
 *
 * @param cur_node Pointer to the node after which the new node will be 
 * inserted.
 * @param new_node Pointer to the new node to be inserted.
 */
static void	insert_in_middle(t_list *cur_node, t_list *new_node)
{
	if (cur_node->next != NULL)
		cur_node->next->prev = new_node;
	new_node->next = cur_node->next;
	cur_node->next = new_node;
	new_node->prev = cur_node;
}

/**
 * @brief Adds a new file to the linked list in alphabetical order.
 *
 * This function creates a new list node containing the file name and 
 * inserts it into the linked list in the
 * correct position to maintain alphabetical order. If the list is empty,
 *  the new node becomes the first element.
 * If the file name is lexicographically less than the first node's name,
 *  the new node is inserted at the beginning.
 * Otherwise, the function finds the appropriate position in the list and
 *  inserts the new node.
 *
 * @param filename The name of the file to be added to the list.
 * @param file_list Pointer to the pointer to the first node of the list.
 * @param shell Pointer to the shell structure used for memory management.
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
 * @brief Cleans up the list of files by removing entries that do not match 
 * the pattern or are hidden.
 *
 * This function filters the provided list of files based on pattern matching 
 * from the substitution buffer (`context->subst_buffer`)
 * and visibility checks. Files that do not match the pattern or are hidden 
 * are removed from the list.
 *
 * @param file_list The list of files to be filtered.
 * @param context The substitution context containing the pattern buffer.
 * @return The cleaned list of files, containing only those that match the 
 * pattern and are not hidden.
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

