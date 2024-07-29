/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:39:35 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:04:36 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes an Abstract Syntax Tree (AST) node.
 *
 * This function processes and executes an AST node based on its type.
 * Possible node types include logical operations, pipelines, subshells,
 * redirections, and commands. The function calls the appropriate functions
 * to execute each type of node.
 *
 * @param ast_node Pointer to the AST node to be executed.
 * @param op_status Operation status that determines whether to terminate
 *                  after execution.
 * @param shell Pointer to the shell structure for managing state and memory.
 * @return The exit status of the node (exit code).
 */
int	run_cmd(t_ast *ast_node, t_op_status op_status, t_shell *shell)
{
	int	result;

	result = EXIT_SUCCESS;
	if (ast_node == NULL)
		return (result);
	if (ast_node->node_type == LOGIC)
		result = process_logic(&ast_node->u_node_cont.logic, shell);
	else if (ast_node->node_type == PIPE)
		result = process_pipeline(ast_node, shell);
	else if (ast_node->node_type == BRACE)
		result = process_brace(&ast_node->u_node_cont.brace, shell);
	else if (ast_node->node_type == REDIR || ast_node->node_type == CMD)
	{
		resolve_ast_content(ast_node, shell);
		if (ast_node->node_type == REDIR)
			result = process_redirection(&ast_node->u_node_cont.redir, shell);
		else if (ast_node->node_type == CMD)
			result = process_cmd(&ast_node->u_node_cont.cmd, op_status,
					shell);
	}
	else
		exit_on_error("run", "unexpected node type", EXIT_FAILURE, shell);
	if (op_status == OP_TERMINATE)
		exit(result);
	return (result);
}

/**
 * @brief Handles the exit status of a child process, returning the appropriate
 *        status code.
 *
 * This function checks the exit status of a child process. If the process
 * exited normally, its exit status is returned. If the process was terminated
 * by a signal, the function writes an error message and, if necessary, adds
 * a newline to the output. If the signal was SIGQUIT or SIGINT, a newline
 * is added if it hasn't been added already.
 *
 * @param child_status The exit status of the child process.
 * @param printed_newline Pointer to a flag indicating whether a newline has
 *                        been added.
 * @param shell Pointer to the shell structure for managing memory and output.
 * @return The exit status of the process. If the process was terminated by
 *         a signal, returns 128 + signal number. If the status does not match
 *         any of these conditions, returns EXIT_FAILURE.
 */
int	handle_exit_signal(int child_status, bool *printed_newline, t_shell *shell)
{
	int		exit_signal;
	bool	need_newline;

	if (!WIFEXITED(child_status))
	{
		if (WIFSIGNALED(child_status))
		{
			exit_signal = WTERMSIG(child_status);
			need_newline = (exit_signal == SIGQUIT || exit_signal == SIGINT);
			if (exit_signal == SIGQUIT)
				write_and_track("Quit with SIGQUIT (3)", STDERR_FILENO, shell);
			if (need_newline)
			{
				if (!printed_newline || !*printed_newline)
				{
					write_and_track("\n", STDERR_FILENO, shell);
					if (printed_newline)
						*printed_newline = true;
				}
			}
			return (128 + exit_signal);
		}
		return (EXIT_FAILURE);
	}
	return (WEXITSTATUS(child_status));
}

/**
 * @brief Executes a command by calling built-in functions or creating a new
 *        process.
 *
 * This function executes a command by either calling built-in shell
 * functions
 * or creating a new process for external commands. If the command is a 
 * built-in
 * command, it is executed directly. If the command requires a new process 
 * and
 * the termination status is set to OP_TERMINATE, the function uses
 *  execute_program
 * to run the command in the current process. Otherwise, the function 
 * creates a
 * new process using create_process, sets standard signals, and executes the
 * command in the child process.
 *
 * @param cmd Pointer to the t_cmd structure containing command arguments.
 * @param term_status Status of the command execution, determining process 
 * termination.
 * @param shell Pointer to the t_shell structure containing shell parameters 
 * and state.
 * @return Returns the execution status of the command: the exit status of a
 *  built-in
 *         function, the exit code of a child process, or an error code.
 */
int	process_cmd(t_cmd *cmd, t_op_status term_status, t_shell *shell)
{
	t_bltn_func	function;
	int			exec_status;

	if (!cmd->cmd_args[0])
		return (EXIT_SUCCESS);
	function = fetch_builtin_cmd(cmd->cmd_args[0]);
	if (function)
		return (function(cmd, shell));
	if (term_status == OP_TERMINATE)
	{
		execute_program(find_executable_path(cmd->cmd_args[0], shell),
			cmd->cmd_args, create_ev_array(shell), shell);
		return (EXIT_FAILURE);
	}
	if (create_process(shell) == 0)
	{
		shell->is_main = false;
		signals_default();
		execute_program(find_executable_path(cmd->cmd_args[0], shell),
			cmd->cmd_args, create_ev_array(shell), shell);
		exit(EXIT_FAILURE);
	}
	wait_for_child(&exec_status, shell);
	return (handle_exit_signal(exec_status, NULL, shell));
}

/**
 * @brief Executes a logical AND or OR operation between two commands.
 *
 * This function executes the first command and saves its status. Depending
 * on the logical operation (AND or OR) and the status of the first command,
 * it may execute the second command and return its status.
 *
 * @param logic Pointer to the logic operation structure containing two commands
 *              and the type of operation.
 * @param shell Pointer to the shell structure for managing process execution.
 * @return The exit status of the last command executed.
 */
int	process_logic(t_logic *logic, t_shell *shell)
{
	int	cmd_status;

	cmd_status = run_cmd(logic->first, OP_COMPLETE, shell);
	shell->prev_cmd_status = cmd_status;
	if ((logic->logic_op == T_AND && cmd_status == EXIT_SUCCESS)
		|| (logic->logic_op == T_OR && cmd_status != EXIT_SUCCESS))
		cmd_status = run_cmd(logic->second, OP_COMPLETE, shell);
	return (cmd_status);
}

/**
 * @brief Executes a command in braces (subshell) in a separate process.
 *
 * This function creates a child process using create_process and executes the
 * command enclosed in braces in that process. The parent process waits for the
 * child process to finish and handles its exit status.
 *
 * @param brace Pointer to the brace structure containing the command.
 * @param shell Pointer to the shell structure for managing process execution.
 * @return The exit status of the executed command.
 */
int	process_brace(t_brace *brace, t_shell *shell)
{
	int		cmd_status;
	bool	is_parent;

	cmd_status = EXIT_FAILURE;
	is_parent = (create_process(shell) != 0);
	if (is_parent)
	{
		wait_for_child(&cmd_status, shell);
		cmd_status = handle_exit_signal(cmd_status, NULL, shell);
	}
	else
	{
		shell->is_main = false;
		signals_default();
		run_cmd(brace->command, OP_TERMINATE, shell);
	}
	return (cmd_status);
}
