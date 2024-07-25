/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:21:21 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/24 08:23:55 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Returns the string representation of a token type.
 *
 * The function `get_tkn_label` takes a token type `tkn_type` and returns
 * its string representation. Depending on the value of `tkn_type`, the
 * corresponding string is returned. If the token type is not recognized,
 * "unsupported token" is returned.
 *
 * @param tkn_type The type of the token (enumeration `t_tkn_type`).
 * @return The string representation of the token type. If the token type
 * is not recognized, returns "unsupported token".
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
 * @brief Returns the value of a token from a list node.
 *
 * The function `get_value` takes a pointer to a token list node `tkn`.
 * It performs the following steps:
 * 1. Initializes the `value` variable to `NULL`.
 * 2. Checks if the token list node `tkn` is not `NULL`.
 * 3. Extracts the content of the list node (assuming it is of type `t_tkn`).
 * 4. Retrieves the token value from the `t_tkn` structure and stores it
 *    in the `value` variable.
 * 5. Returns the token value.
 *
 * @param tkn Pointer to a token list node.
 * @return Pointer to the string containing the token value, or `NULL`
 *         if the list node is empty.
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
 * @brief Returns the type of a token from a list node.
 *
 * The function `get_type` takes a pointer to a token list node `tkn_node`.
 * It performs the following steps:
 * 1. Extracts the content of the list node (assuming it is of type `t_tkn`).
 * 2. Returns the token type stored in the `t_tkn` structure.
 *
 * @param tkn_node Pointer to a token list node.
 * @return The value of type `t_tkn_type` representing the token type.
 */
t_tkn_type	get_type(t_list *tkn_node)
{
	t_tkn	*tkn_content;

	tkn_content = (t_tkn *)tkn_node->content;
	return (tkn_content->type);
}

/**
 * @brief Parses a command and creates an AST node for the command.
 *
 * The function `parse_cmd` takes a list of tokens `tokens` and a `shell` 
 * structure. It performs the following steps:
 * 1. Initializes variables: `arg_count` for counting arguments, `i` for 
 *    iteration, `cur_tkn` for the current token, and `cmd_node` for the 
 *    command node.
 * 2. Iterates through tokens and counts the number of command arguments
 *    until a non-text token is encountered.
 * 3. If no arguments are found (i.e., `arg_count` is 0), returns `NULL`.
 * 4. Allocates memory for the command argument array (`args`), including
 *    space for a terminating `NULL`.
 * 5. Fills the `args` array with token values and advances the current token
 *    pointer (`tokens`).
 * 6. Creates an AST node for the command using `build_node_cmd` and returns it.
 *
 * @param tokens Pointer to the list of tokens.
 * @param shell Shell structure.
 * @return Pointer to the command AST node, or `NULL` if no arguments are found.
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
 * @brief Parses logical expressions and builds the corresponding AST.
 *
 * The function `parse_logic` parses logical operators (&& and ||) in a command
 * and builds an AST that represents the logical structure of the command. It 
 * recursively parses expressions with logical operators and connects them into
 * a tree.
 *
 * @param tkn_list Pointer to the list of tokens representing the command.
 * @param shell Pointer to the shell structure containing shell state 
 * information.
 * @return Pointer to the root of the AST representing the logical expression.
 *
 * The function performs the following steps:
 * 1. Parses the first subexpression using `parse_pipe` and saves it as 
 * `first_expr`.
 * 2. In a loop, checks if there are more tokens to parse and if they are 
 * logical
 *    operators (&& or ||).
 * 3. If the current token is a logical operator, saves its type in 
 * `operator_type`
 *    and moves to the next token.
 * 4. Parses the second subexpression using `parse_pipe` and saves it 
 * as `second_expr`.
 * 5. If the second subexpression fails to parse, returns the first 
 * subexpression.
 * 6. Creates a new logical operator node using `build_node_logic`, 
 * passing in the
 *    first subexpression, operator type, and second subexpression.
 * 7. If node creation fails, returns the first subexpression.
 * 8. Updates `first_expr` to the new logical node and continues the loop.
 * 9. Returns the root of the AST representing the logical expression.
 */
t_ast	*parse_logic(t_list **tkn_list, t_shell *shell)
{
	t_ast		*first_expr;
	t_ast		*second_expr;
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
 * @brief Parses commands with pipes and builds the corresponding AST.
 *
 * The function `parse_pipe` parses pipe operators (|) in a command and builds
 * an AST (Abstract Syntax Tree) that represents a chain of commands connected
 * by pipes. It recursively parses commands with pipes and connects them 
 * into a tree.
 *
 * @param tkn_list Pointer to the list of tokens representing the command.
 * @param shell Pointer to the shell structure containing shell state 
 * information.
 * @return Pointer to the root of the AST representing the command chain 
 * with pipes.
 *
 * The function performs the following steps:
 * 1. Parses the first subexpression with redirects using `process_redir` and 
 * saves it as `input_side`.
 * 2. In a loop, checks if there are more tokens to parse and if they are 
 * pipe operators (|).
 * 3. If the current token is a pipe operator, moves to the next token.
 * 4. Parses the second subexpression with redirects using `process_redir` 
 * and saves it as `output_side`.
 * 5. If the second subexpression fails to parse, returns the first 
 * subexpression.
 * 6. Creates a new pipe node using `build_node_pipe`, passing in the 
 * first and second subexpressions.
 * 7. If node creation fails, returns the first subexpression.
 * 8. Updates `input_side` to the new pipe node and continues the loop.
 * 9. Returns the root of the AST representing the command chain with pipes.
 */

t_ast	*parse_pipe(t_list **tkn_list, t_shell *shell)
{
	t_ast	*input_side;
	t_ast	*output_side;
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

/**
 * @brief Parses tokens enclosed in braces and creates corresponding AST nodes.
 *
 * This function handles tokens enclosed in braces (T_BRACE_START and 
 * T_BRACE_END).
 * It checks for the presence of an opening brace, then calls `parse_logic` 
 * to parse
 * the contents of the braces. Afterward, it checks for the presence of a 
 * closing brace.
 * If a syntax error is detected, it records the error and returns `NULL`.
 *
 * @param tokens Pointer to the list of tokens.
 * @param shell Pointer to the shell structure.
 * @return Pointer to the AST node representing the contents of the braces, 
 * or `NULL`
 *         in case of a syntax error.
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
 * @brief Parses a list of tokens and creates a syntax tree (AST).
 *
 * The function `parse_tokens` takes a list of tokens `tokens` and a `shell` 
 * structure.
 * It performs the following steps:
 * 1. Calls `parse_logic` to create an AST from the tokens.
 * 2. If there are leftover tokens after parsing, records a syntax error 
 * with the first
 *    unused token.
 * 3. If a syntax error is detected during parsing, displays it and returns 
 * the error result.
 * 4. If no errors are found, returns `EXIT_SUCCESS`.
 *
 * @param tokens Pointer to the list of tokens.
 * @param syntax_tree Pointer to a pointer for the syntax tree.
 * @param shell Shell structure.
 * @return `EXIT_SUCCESS` on success, error code on syntax error.
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
