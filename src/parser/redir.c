/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:24:48 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:06:39 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the current token is a valid redirection or text that
 *  is followed by a command.
 *
 * The `is_valid_redir` function takes a pointer to the list of tokens and 
 * a command node `cmd_node`.
 * It performs the following steps:
 * 1. If `tokens` or the current token is `NULL`, returns `false`.
 * 2. Gets the type of the current token.
 * 3. If the token type is `T_TEXT` and a command node `cmd_node` is present,
 *  returns `true`.
 * 4. If the token type is `T_APPEND`, `T_HDOC`, `T_INPUT`, or `T_OUTPUT`, 
 * returns `true`.
 * 5. In all other cases, returns `false`.
 *
 * @param tokens Pointer to the list of tokens.
 * @param cmd_node Command node.
 * @return `true` if the token is valid, otherwise `false`.
 */
bool	is_valid_redir(t_list **tokens, t_ast *cmd_node)
{
	t_tkn_type	type;

	if (tokens == NULL || *tokens == NULL)
		return (false);
	type = get_type(*tokens);
	if (cmd_node && type == T_TEXT)
		return (true);
	if (type == T_APPEND || type == T_HDOC || type == T_INPUT
		|| type == T_OUTPUT)
		return (true);
	return (false);
}

/**
 * @brief Adds a new redirection node to the redirection chain.
 *
 * The `append_redir_node` function takes two AST nodes: 
 * `redir_chain` and `new_redir`.
 * If `redir_chain` is `NULL`, it returns `new_redir`.
 * Otherwise, it traverses the redirection chain `redir_chain` 
 * to the end and appends `new_redir`.
 *
 * @param redir_chain Start of the redirection node chain.
 * @param new_redir New redirection node to be added.
 * @return Start of the redirection node chain with the added node.
 */
t_ast	*append_redir_node(t_ast *redir_chain, t_ast *new_redir)
{
	t_ast	*current_redir;

	if (redir_chain == NULL)
		return (new_redir);
	current_redir = redir_chain;
	while (current_redir->u_node_cont.redir.command)
		current_redir = current_redir->u_node_cont.redir.command;
	current_redir->u_node_cont.redir.command = new_redir;
	return (redir_chain);
}

/**
 * @brief Extracts and processes redirections from the list of 
 * tokens.
 *
 * The `extract_redirections` function iterates through the list 
 * of tokens `tokens`,
 * extracts redirection nodes, adds them to the redirection chain, 
 * and returns the chain.
 * If the token is a command argument, it is added to the `cmd_node`.
 * When a redirection is found, a new redirection node is created and 
 * added to the chain.
 *
 * @param tokens Pointer to the list of tokens.
 * @param cmd_node Command node to which arguments will be added.
 * @param shell Shell structure.
 * @return Start of the redirection node chain or `NULL` in case of an error.
 */
t_ast	*extract_redirections(t_list **tokens, t_ast *cmd_node, t_shell *shell)
{
	t_ast	*redir_chain;
	t_ast	*new_redir;

	redir_chain = NULL;
	while (is_valid_redir(tokens, cmd_node))
	{
		if (add_cmd_arg(tokens, cmd_node, shell))
			continue ;
		new_redir = build_node_redir(get_type(*tokens), (*tokens)->next, NULL,
				shell);
		if (new_redir == NULL)
			return (NULL);
		redir_chain = append_redir_node(redir_chain, new_redir);
		*tokens = (*tokens)->next->next;
	}
	return (redir_chain);
}
