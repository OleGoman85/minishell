/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:32:14 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 12:12:13 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
static int	add_token(char *input, size_t len, t_tkn_type type,
		t_list **tokens, t_shell *shell)
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
