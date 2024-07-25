/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:32:14 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 07:44:36 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if a character is a whitespace character.
 *
 * This function checks if the given character is a whitespace character 
 * including space, tab, vertical tab, newline, carriage return, or form feed.
 *
 * @param c The character to check.
 * @return 1 if the character is a whitespace character, otherwise 0.
 */
int	is_space(int c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r'
		|| c == '\f')
		return (1);
	return (0);
}

/**
 * @brief Checks if a character is a special shell token.
 *
 * This function checks if the given character is one of the special shell tokens
 * including '(', ')', '<', '>', '&', or '|'.
 *
 * @param c The character to check.
 * @return 1 if the character is a special token, otherwise 0.
 */
int	is_special_tkn(int c)
{
	if (c == '(' || c == ')' || c == '<' || c == '>' || c == '&' || c == '|')
	{
		return (1);
	}
	return (0);
}
/**
 * @brief Records a syntax error in the shell structure.
 *
 * If there is no syntax error currently recorded in the shell structure,
 * this function records the given token as the syntax error.
 *
 * @param tkn The token that caused the syntax error.
 * @param shell Pointer to the shell structure.
 * @return Always returns NULL.
 */

void	*record_synt_err(char *tkn, t_shell *shell)
{
	if (shell->syntax_error == NULL)
		shell->syntax_error = tkn;
	return (NULL);
}

/**
 * @brief Displays the recorded syntax error message.
 *
 * This function displays the syntax error message stored in the shell structure
 * and resets the syntax error field to NULL.
 *
 * @param shell Pointer to the shell structure.
 * @return Always returns 2 to indicate a syntax error.
 */
int	display_synt_err(t_shell *shell)
{
	error_msg("syntax error near unexpected token `", shell->syntax_error, "'",
		shell);
	shell->syntax_error = NULL;
	return (2);
}

/**
 * @brief Determines the token type of the input string.
 *
 * This function identifies the type of the token at the beginning of
 * the input string
 * and updates the length of the token. It handles various token types such as 
 * ampersands, pipes, redirects, and braces.
 *
 * @param input The input string to tokenize.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return The type of the token.
 */
t_tkn_type	get_tkn_type(char *input, size_t *len, t_shell *shell)
{
	if (*input == '&')
		return (get_ampersand(input, len, shell));
	else if (*input == '|')
		return (get_pipe(input, len, shell));
	else if (*input == '<' || *input == '>')
		return (get_redirect(input, *input, len, shell));
	else if (*input == '(' || *input == ')')
		return (get_braces(input, len));
	return (get_word(input, len, shell));
}

/**
 * @brief Determines the type of brace token and updates the token length.
 *
 * This function identifies if the token is a starting
 * brace '(' or an ending brace ')'
 * and updates the length of the token.
 *
 * @param input The input string containing the brace token.
 * @param len Pointer to the length of the token.
 * @return T_BRACE_START if '(', T_BRACE_END if ')'.
 */
t_tkn_type	get_braces(char *input, size_t *len)
{
	*len += 1;
	if (*input == '(')
		return (T_BRACE_START);
	return (T_BRACE_END);
}

/**
 * @brief Determines the type of ampersand token and updates the token length.
 *
 * This function identifies if the token is a single '&' or a double '&&' 
 * and updates the length of the token. If the token is invalid, it records a 
 * syntax error.
 *
 * @param input The input string containing the ampersand token.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return T_AND if '&&', T_ERR if invalid token.
 */
t_tkn_type	get_ampersand(char *input, size_t *len, t_shell *shell)
{
	while (input[*len] == '&')
		(*len)++;
	if (*len == 2)
		return (T_AND);
	record_synt_err("&", shell);
	return (T_ERR);
}
/**
 * @brief Determines the type of pipe token and updates the token length.
 *
 * This function identifies if the token is a single '|' or a double '||'
 * and updates the length of the token. If the token is invalid, it records a 
 * syntax error.
 *
 * @param input The input string containing the pipe token.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return T_PIPE if '|', T_OR if '||', T_ERR if invalid token.
 */

t_tkn_type	get_pipe(char *input, size_t *len, t_shell *shell)
{
	while (input[*len] == '|')
		(*len)++;
	if (*len == 1)
		return (T_PIPE);
	if (*len == 2)
		return (T_OR);
	record_synt_err("||", shell);
	return (T_ERR);
}

/**
 * @brief Determines the type of redirect token and updates the token length.
 *
 * This function identifies if the token is an input '<', here-doc '<<',
 * output '>', or append '>>' and updates the length of the token.
 * If the token is invalid, it records a syntax error.
 *
 * @param input The input string containing the redirect token.
 * @param c The character representing the redirect type.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return The type of the redirect token.
 */
t_tkn_type	get_redirect(char *input, char c, size_t *len, t_shell *shell)
{
	while (input[*len] == c)
		*len += 1;
	if (c == '<' && *len == 1)
		return (T_INPUT);
	else if (c == '<' && *len == 2)
		return (T_HDOC);
	else if (c == '>' && *len == 1)
		return (T_OUTPUT);
	else if (c == '>' && *len == 2)
		return (T_APPEND);
	record_synt_err(input, shell);
	return (T_ERR);
}

// /**
//  * @brief Determines the type of word token and updates the token length.
//  *
//  * This function identifies a word token, handling escape characters and quotes.
//  * It updates the token length and checks for unclosed quotes. If there is an 
//  * unclosed quote, it records a syntax error.
//  *
//  * @param input The input string containing the word token.
//  * @param len Pointer to the length of the token.
//  * @param shell Pointer to the shell structure.
//  * @return T_TEXT if the token is a word, T_ERR if there is an unclosed quote.
//  */
// t_tkn_type	get_word(char *input, size_t *len, t_shell *shell)
// {
// 	char	c;
// 	bool	in_quote;

// 	in_quote = false;
// 	while (input[*len])
// 	{
// 		if (input[*len + 1] && input[*len] == '\\')
// 			*len += 1;
// 		else if (input[*len] == '\"' || input[*len] == '\'')
// 		{
// 			if (!in_quote)
// 			{
// 				c = input[*len];
// 				in_quote = true;
// 			}
// 			else if (in_quote && input[*len] == c)
// 				in_quote = false;
// 		}
// 		else if ((is_special_tkn(input[*len]) || is_space(input[*len]))
// 			&& !in_quote)
// 			break ;
// 		*len += 1;
// 	}
// 	if (in_quote)
// 		return (record_synt_err("unclosed quote", shell), T_ERR);
// 	return (T_TEXT);
// }

/**
 * @brief Handles quotes and escape characters while determining token length.
 *
 * This function processes the input string to handle quotes and escape 
 * characters,
 * updating the token length accordingly.
 *
 * @param input The input string containing the token.
 * @param len Pointer to the length of the token.
 * @param in_quote Pointer to a boolean indicating if inside a quote.
 * @param c Pointer to the current quote character.
 */
void	handle_quotes_and_escapes(char *input, size_t *len,
		bool *in_quote, char *c)
{
	while (input[*len])
	{
		if (input[*len + 1] && input[*len] == '\\')
			*len += 1;
		else if (input[*len] == '\"' || input[*len] == '\'')
		{
			if (!*in_quote)
			{
				*c = input[*len];
				*in_quote = true;
			}
			else if (*in_quote && input[*len] == *c)
				*in_quote = false;
		}
		else if ((is_special_tkn(input[*len]) || is_space(input[*len]))
			&& !*in_quote)
			break ;
		*len += 1;
	}
}

/**
 * @brief Determines the type of word token and updates the token length.
 *
 * This function identifies a word token, handling escape characters and 
 * quotes.
 * It updates the token length and checks for unclosed quotes. If there is an 
 * unclosed quote, it records a syntax error.
 *
 * @param input The input string containing the word token.
 * @param len Pointer to the length of the token.
 * @param shell Pointer to the shell structure.
 * @return T_TEXT if the token is a word, T_ERR if there is an unclosed quote.
 */
t_tkn_type	get_word(char *input, size_t *len, t_shell *shell)
{
	char	c;
	bool	in_quote;

	in_quote = false;
	handle_quotes_and_escapes(input, len, &in_quote, &c);
	if (in_quote)
		return (record_synt_err("unclosed quote", shell), T_ERR);
	return (T_TEXT);
}

/**
 * @brief Adds a token to the token list.
 *
 * This function creates a new token of the specified type and value,
 * then adds it to the end of the token list.
 *
 * @param input The input string containing the token.
 * @param len The length of the token.
 * @param type The type of the token.
 * @param tokens Pointer to the list of tokens.
 * @param shell Pointer to the shell structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
static int	add_token(char *input, size_t len, t_tkn_type type, t_list **tokens, t_shell *shell)
{
	t_tkn	*token;

	token = create_token(type, input, len, shell);
	if (!token)
		return (EXIT_FAILURE);
	lstadd_back_tracked(token, tokens, COMMAND_TRACK, shell);
	return (EXIT_SUCCESS);
}

/**
 * @brief Tokenizes the input string into a list of tokens.
 *
 * This function processes the input string, identifies each token,
 * and adds it to the token list. It handles different types of tokens
 * including words, special characters, and quotes.
 *
 * @param input_str The input string to tokenize.
 * @param tokens Pointer to the list of tokens.
 * @param shell Pointer to the shell structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE or syntax error code on error.
 */
int	tokenize_input(char *input_str, t_list **tokens, t_shell *shell)
{
	size_t		i;
	size_t		token_length;
	t_tkn_type	token_type;

	i = 0;
	*tokens = NULL;
	while (input_str[i])
	{
		while (is_space(input_str[i]))
			i++;
		token_length = 0;
		token_type = get_tkn_type(input_str + i, &token_length, shell);
		if (token_type == T_TEXT)
			token_type = get_word(input_str + i, &token_length, shell);
		if (token_type == T_ERR)
			return (display_synt_err(shell));
		if (add_token(input_str + i, token_length, token_type, tokens,
				shell) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		i += token_length;
	}
	return (EXIT_SUCCESS);
}
/**
 * @brief Creates a new token and allocates memory for it.
 *
 * This function creates a new token of the specified type and value,
 * allocates memory for it, and manages the memory allocation tracking.
 *
 * @param tkn_type The type of the token.
 * @param tkn_value The value of the token.
 * @param tkn_len The length of the token.
 * @param shell Pointer to the shell structure.
 * @return Pointer to the newly created token.
 */

t_tkn	*create_token(t_tkn_type tkn_type, char *tkn_value, size_t tkn_len,
		t_shell *shell)
{
	t_tkn	*new_tkn;

	new_tkn = calloc_tracked(1, sizeof(t_tkn), COMMAND_TRACK, shell);
	new_tkn->type = tkn_type;
	new_tkn->value = manage_memory(ft_substr(tkn_value, 0, tkn_len),
			COMMAND_TRACK, shell);
	return (new_tkn);
}

/**
 * @brief Processes the input string to execute commands in the shell.
 *
 * This function tokenizes the input string, parses the tokens into an
 * abstract syntax tree (AST), and runs the commands. It handles errors
 * at each stage and performs necessary cleanup.
 *
 * @param input_str The input string containing the commands.
 * @param shell Pointer to the shell structure.
 * @return EXIT_SUCCESS on success, error code on failure.
 */
int	process_input(char *input_str, t_shell *shell)
{
	t_list	*tokens;
	t_ast	*parse_tree;
	int		result;

	tokens = NULL;
	parse_tree = NULL;
	result = tokenize_input(input_str, &tokens, shell);
	if (result != EXIT_SUCCESS || tokens == NULL)
		return (result);
	result = parse_tokens(tokens, &parse_tree, shell);
	if (result != EXIT_SUCCESS || parse_tree == NULL)
		return (result);
	result = run_cmd(parse_tree, OP_COMPLETE, shell);
	return (result);
}
