/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:10:20 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:04:58 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a sequence of commands connected by pipes.
 *
 * This function creates pipes, executes each command in the command list,
 * and waits for all child processes to complete. It uses file descriptors to
 * pass data between commands in the pipeline.
 *
 * @param pipe_lst Pointer to the list of commands to be executed in 
 * the pipeline.
 * @param shell Pointer to the shell structure for process management.
 * @return The exit code of the last command in the pipeline.
 */
int	execute_command_chain(t_list *pipe_lst, t_shell *shell)
{
	int		pipe_fds[2];
	int		last_pipe_fd;
	pid_t	last_exec_pid;
	t_list	*current;

	last_exec_pid = 0;
	last_pipe_fd = -1;
	current = pipe_lst;
	while (current)
	{
		create_pipe(pipe_fds, shell);
		last_exec_pid = run_pipe_cmd(current, last_pipe_fd, pipe_fds,
				shell);
		if (last_pipe_fd != -1)
			close_file(last_pipe_fd, shell);
		close_file(pipe_fds[PIPE_OUTPUT], shell);
		last_pipe_fd = pipe_fds[PIPE_INPUT];
		current = current->next;
	}
	return (collect_child_statuses(last_exec_pid, ft_lstsize(pipe_lst), shell));
}
/**
 * @brief Collects and returns the final exit status of all pipeline processes.
 *
 * This function waits for all pipeline processes to complete, processes their
 * exit status, and returns the exit status of the last executed process.
 * Additional messages and newlines are handled when processing signal statuses.
 *
 * @param last_exec_pid PID of the last executed process.
 * @param pipe_count Number of processes in the pipeline.
 * @param shell Pointer to the shell structure for memory and signal management.
 * @return The final exit status of the last executed process.
 */

int	collect_child_statuses(pid_t last_exec_pid, int pipe_count, t_shell *shell)
{
	bool	need_newline;
	int		sig_status;
	int		exit_status;
	int		result;
	pid_t	current_pid;

	need_newline = false;
	result = 0;
	while (pipe_count > 0)
	{
		current_pid = wait_for_child(&sig_status, shell);
		exit_status = handle_exit_signal(sig_status, &need_newline, shell);
		if (current_pid == last_exec_pid)
			result = exit_status;
		pipe_count--;
	}
	return (result);
}

/**
 * @brief Processes and executes a command pipeline.
 *
 * This function creates a list of commands from the provided AST node and
 * calls the function to execute the command chain. It returns the pipeline
 * execution status.
 *
 * @param ast_node Pointer to the AST node representing the pipeline.
 * @param shell Pointer to the shell structure used for memory and state 
 * management.
 * @return The status of the pipeline execution.
 */
int	process_pipeline(t_ast *ast_node, t_shell *shell)
{
	t_list	*commands;
	t_ast	*current;
	int		status;

	commands = NULL;
	current = ast_node;
	while (current && current->node_type == PIPE)
	{
		lstadd_front_tracked(current->u_node_cont.pipe.output_side, &commands,
			COMMAND_TRACK, shell);
		current = current->u_node_cont.pipe.input_side;
	}
	if (current)
		lstadd_front_tracked(current, &commands, COMMAND_TRACK, shell);
	status = execute_command_chain(commands, shell);
	return (status);
}

/**
 * @brief Executes a single command in the pipeline.
 *
 * This function executes one command in the pipeline by creating a new process
 * using `create_process`. In the child process, input/output streams are 
 * redirected
 * to create pipes, and then the command is executed. The parent process returns
 * the PID of the child process.
 *
 * @param pipe_lst Pointer to the list of pipeline commands.
 * @param last_pipe_fd Read descriptor from the previous pipe.
 * @param pipe_fds Array of two descriptors for the current pipe.
 * @param shell Pointer to the shell structure for state management.
 * @return PID of the child process executing the command.
 */
pid_t	run_pipe_cmd(t_list *pipe_lst, int last_pipe_fd,
		int pipe_fds[2], t_shell *shell)
{
	pid_t	last_exec_pid;
	t_ast	*current_ast;
	bool	is_not_last_command;

	current_ast = (t_ast *)pipe_lst->content;
	last_exec_pid = create_process(shell);
	if (last_exec_pid == 0)
	{
		shell->is_main = false;
		signals_default();
		if (last_pipe_fd != -1)
		{
			duplicate_fd(last_pipe_fd, STDIN_FILENO, shell);
			close_file(last_pipe_fd, shell);
		}
		is_not_last_command = (pipe_lst->next != NULL);
		if (is_not_last_command)
			duplicate_fd(pipe_fds[PIPE_OUTPUT], STDOUT_FILENO, shell);
		close_file(pipe_fds[PIPE_INPUT], shell);
		close_file(pipe_fds[PIPE_OUTPUT], shell);
		run_cmd(current_ast, OP_TERMINATE, shell);
	}
	return (last_exec_pid);
}
