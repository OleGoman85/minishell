/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:30:59 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:05:10 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates an AST node for a logical operation and initializes it.
 *
 * This function takes two expressions (`first_expr` and `second_expr`) and a 
 * logical operation type (`op`).
 * If either of the expressions is NULL, the function records a syntax error 
 * and returns NULL.
 *
 * 1. Checks that both expressions (`first_expr` and `second_expr`) are 
 * not NULL.
 * 2. If either expression is NULL, records a syntax error and returns NULL.
 * 3. Allocates memory for a new node of type LOGIC.
 * 4. Initializes the new node, setting its type to LOGIC and linking the 
 * logical operation (`op`)
 *    with the `first_expr` and `second_expr` fields.
 * 5. Returns the created node.
 *
 * @param first_expr Pointer to the node representing the first expression.
 * @param op Type of the logical operation.
 * @param second_expr Pointer to the node representing the second expression.
 * @param shell Shell structure used for memory management and syntax 
 * error handling.
 * @return Pointer to the created logical operation node or NULL in case 
 * of an error.
 */
t_ast	*build_node_logic(t_ast *first_expr, t_tkn_type op, t_ast *second_expr,
		t_shell *shell)
{
	t_ast	*logic_node;

	if (first_expr && second_expr)
	{
		logic_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (logic_node)
		{
			logic_node->node_type = LOGIC;
			logic_node->u_node_cont.logic.logic_op = op;
			logic_node->u_node_cont.logic.first = first_expr;
			logic_node->u_node_cont.logic.second = second_expr;
		}
		return (logic_node);
	}
	else
		return (record_synt_err(get_tkn_label(op), shell));
}

/**
 * @brief Creates an AST node for a command and initializes it.
 *
 * This function takes an array of command arguments (`cmd_args`).
 * It allocates memory for a new node of type CMD and initializes it.
 *
 * 1. Allocates memory for a new node of type CMD using the setup_node function.
 * 2. Initializes the new node, setting its type to CMD and linking the command
 * arguments (`cmd_args`).
 * 3. Returns the created node.
 *
 * @param cmd_args Array of command arguments.
 * @param shell Shell structure used for memory management.
 * @return Pointer to the created command node or NULL in case of an error.
 */
t_ast	*build_node_cmd(char **cmd_args, t_shell *shell)
{
	t_ast	*cmd_node;

	cmd_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
	if (cmd_node)
	{
		cmd_node->node_type = CMD;
		cmd_node->u_node_cont.cmd.cmd_args = cmd_args;
	}
	return (cmd_node);
}

/**
 * @brief Creates an AST node for a pipe and initializes it.
 *
 * This function takes two nodes (`input_node` and `output_node`) that will 
 * be connected via a pipe.
 * If either node is NULL, the function records a syntax error and returns 
 * NULL.
 *
 * 1. Checks that both nodes (`input_node` and `output_node`) are not NULL.
 * 2. If either node is NULL, records a syntax error and returns NULL.
 * 3. Allocates memory for a new node of type PIPE.
 * 4. Initializes the new node, setting its type to PIPE and linking the 
 * input and output nodes
 *    with the `input_side` and `output_side` fields, respectively.
 * 5. Returns the created node.
 *
 * @param input_node Pointer to the node representing the input side of 
 * the pipe.
 * @param output_node Pointer to the node representing the output side 
 * of the pipe.
 * @param shell Shell structure.
 * @return Pointer to the created pipe node or NULL in case of an error.
 */
t_ast	*build_node_pipe(t_ast *input_node, t_ast *output_node, t_shell *shell)
{
	t_ast	*pipe_node;

	if (input_node && output_node)
	{
		pipe_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (pipe_node)
		{
			pipe_node->node_type = PIPE;
			pipe_node->u_node_cont.pipe.input_side = input_node;
			pipe_node->u_node_cont.pipe.output_side = output_node;
		}
		return (pipe_node);
	}
	return (record_synt_err("|", shell));
}

/**
 * @brief Creates an AST node for a redirection.
 *
 * @param redir_type Type of the redirection.
 * @param token Token containing the filename.
 * @param child_node Node representing the command to which the redirection 
 * is applied.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the created AST node for the redirection or NULL in 
 * case of an error.
 */
t_ast	*build_node_redir(t_tkn_type redir_type, t_list *token,
		t_ast *child_node, t_shell *shell)
{
	t_ast	*redir_node;
	char	*filename;

	if (token && get_type(token) == T_TEXT)
	{
		filename = get_value(token);
		if (filename)
		{
			redir_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
			if (redir_node)
			{
				redir_node->node_type = REDIR;
				redir_node->u_node_cont.redir.filename = filename;
				redir_node->u_node_cont.redir.redir_type = redir_type;
				redir_node->u_node_cont.redir.command = child_node;
			}
			return (redir_node);
		}
	}
	record_synt_err(get_tkn_label(redir_type), shell);
	return (NULL);
}

/**
 * @brief Creates an AST node for a grouping command (in braces).
 *
 * @param inner_node AST node representing the command inside the braces.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the created AST node for the grouping command or NULL
 *  in case of an error.
 */
t_ast	*build_node_brace(t_ast *inner_node, t_shell *shell)
{
	t_ast	*brace_node;

	if (inner_node != NULL)
	{
		brace_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
		if (brace_node)
		{
			brace_node->node_type = BRACE;
			brace_node->u_node_cont.brace.command = inner_node;
		}
		return (brace_node);
	}
	record_synt_err("(", shell);
	return (NULL);
}
