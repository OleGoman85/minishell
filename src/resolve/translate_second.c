/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_second.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:47:14 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 13:50:03 by ogoman           ###   ########.fr       */
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
