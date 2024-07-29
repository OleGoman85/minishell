/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:32:14 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:24:59 by aarbenin         ###   ########.fr       */
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
 * @brief Tokenizes the input string and adds tokens to the list.
 *
 * This function processes the input string, identifies tokens, 
 * and adds them
 * to the provided token list. It handles spaces, token types, 
 * and errors during tokenization.
 *
 * @param input_str The input string to be tokenized.
 * @param tokens Pointer to the list of tokens.
 * @param shell Pointer to the shell structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int	tokenize_input(char *input_str, t_list **tokens, t_shell *shell)
{
	size_t		i;
	size_t		token_length;
	t_tkn_type	token_type;
	t_tkn		*token;

	i = 0;
	*tokens = NULL;
	while (input_str[i])
	{
		while (ft_isspace(input_str[i]))
			i++;
		token_length = 0;
		token_type = get_tkn_type(input_str + i, &token_length, shell);
		if (token_type == T_TEXT)
			token_type = get_word(input_str + i, &token_length, shell);
		if (token_type == T_ERR)
			return (display_synt_err(shell));
		token = create_token(token_type, input_str + i, token_length, shell);
		if (!token)
			return (EXIT_FAILURE);
		lstadd_back_tracked(token, tokens, COMMAND_TRACK, shell);
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
