/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redic_second.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:51:49 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:06:30 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Links a command to a sequence of redirections.
 *
 * The `link_redir_to_cmd` function takes a sequence of redirections 
 * `redir_sequence`
 * and a command `cmd`. It finds the last redirection in the sequence 
 * and attaches
 * it to the command `cmd`.
 *
 * @param redir_sequence Pointer to the AST node representing the sequence 
 * of redirections.
 * @param cmd Pointer to the AST node representing the command.
 * @return Pointer to the updated AST node of the redirection sequence.
 */

t_ast	*link_redir_to_cmd(t_ast *redir_sequence, t_ast *cmd)
{
	t_ast	*redir_tail;

	redir_tail = find_redir_tail(redir_sequence);
	redir_tail->u_node_cont.redir.command = cmd;
	return (redir_sequence);
}

/**
 * @brief Assembles a command with redirection prefixes and suffixes.
 *
 * The `assemble_cmd` function takes a redirection prefix `redir_prefix`,
 * a redirection suffix `redir_suffix`, and a command `cmd`. It combines 
 * all parts
 * to ensure the command sequence is coherent and correct.
 *
 * @param redir_prefix Pointer to the AST node representing the redirection 
 * prefix.
 * @param redir_suffix Pointer to the AST node representing the redirection 
 * suffix.
 * @param cmd Pointer to the AST node representing the command.
 * @return Pointer to the AST node representing the assembled command with 
 * redirections.
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
 * @brief Finds and returns the last redirection node in a redirection chain.
 *
 * The function takes a pointer to the first node of a redirection chain
 * (`redir_head`)
 * and traverses the chain until it reaches the last redirection node. 
 * Redirection nodes
 * are identified by their type `REDIR`, and the end of the chain is 
 * determined either
 * by the absence of a redirection command or by encountering a command 
 * that is not a redirection.
 *
 * @param redir_head Pointer to the first node in the redirection chain.
 * @return Pointer to the last redirection node in the chain, or the last node 
 * that is not a redirection,
 *         if the chain ends with a command.
 */
t_ast	*find_redir_tail(t_ast *redir_head)
{
	t_ast	*current_node;

	current_node = redir_head;
	while (current_node != NULL)
	{
		if (current_node->node_type != REDIR)
			break ;
		if (current_node->u_node_cont.redir.command == NULL)
			break ;
		if (current_node->u_node_cont.redir.command->node_type != REDIR)
			break ;
		current_node = current_node->u_node_cont.redir.command;
	}
	return (current_node);
}

/**
 * @brief Processes redirections and creates an AST node for the command 
 * with redirections.
 *
 * The `process_redir` function performs the following steps:
 * 1. Extracts the redirections preceding the command (`initial_redir`).
 * 2. If a syntax error is detected, returns `NULL`.
 * 3. Parses the command wrapped in braces using `parse_brace`.
 * 4. If the command is not found or a syntax error is detected, returns 
 * `initial_redir`.
 * 5. Extracts the redirections following the command (`end_redir`).
 * 6. If a syntax error is detected, returns `initial_redir`.
 * 7. If there are subsequent redirections, combines them with the command 
 * and returns the result.
 * 8. If there are only initial redirections, combines them with the 
 * command and returns the result.
 * 9. If there are no redirections, returns the command node.
 *
 * @param tokens Pointer to the list of tokens.
 * @param shell Shell structure.
 * @return Pointer to the AST node for the command with redirections, or 
 * `NULL` if a syntax error is detected.
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

/**
 * @brief Adds a command argument to the command AST node if the current 
 * token is text.
 *
 * The `add_cmd_arg` function performs the following steps:
 * 1. Checks that the provided pointers `tokens`, `cmd_node`, and `shell` 
 * are not `NULL`.
 * 2. Gets the type of the current token.
 * 3. If the token type is not text (`T_TEXT`), returns `false`.
 * 4. Retrieves the value of the current token and appends it to the 
 * command arguments array
 *    of the `cmd_node` AST node.
 * 5. Advances the `tokens` pointer to the next token.
 * 6. Returns `true` if the command argument was successfully added.
 *
 * @param tokens Pointer to the list of tokens.
 * @param cmd_node Pointer to the AST node for the command.
 * @param shell Shell structure.
 * @return `true` if the command argument was successfully added, 
 * otherwise `false`.
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
	append_str(&cmd_node->u_node_cont.cmd.cmd_args, token_value, shell);
	*tokens = (*tokens)->next;
	return (true);
}
