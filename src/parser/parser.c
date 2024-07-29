/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:21:21 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:06:17 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
