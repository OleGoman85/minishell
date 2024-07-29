/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:13:46 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:05:02 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles input redirection.
 *
 * This function opens a file for reading, saves the current stdin,
 * redirects stdin to the opened file, executes the command, restores
 * the original stdin, and closes the file. If the file cannot be opened,
 * it returns an error code.
 *
 * @param redirect The redirection structure containing file and command 
 * information.
 * @param shell Pointer to the shell structure for process management.
 * @return The exit code of the command or an error code.
 */
int	process_input_redirection(t_redir *redirect, t_shell *shell)
{
	int		fd;
	int		exit_code;
	int		stdin_temp;
	bool	success;

	fd = open(redirect->filename, O_RDONLY);
	success = (fd != -1);
	if (success)
	{
		stdin_temp = dup(STDIN_FILENO);
		success = (duplicate_fd(fd, STDIN_FILENO, shell) != -1);
		close_file(fd, shell);
	}
	if (success)
	{
		exit_code = run_cmd(redirect->command, OP_COMPLETE, shell);
		duplicate_fd(stdin_temp, STDIN_FILENO, shell);
		close_file(stdin_temp, shell);
	}
	else
		exit_code = error_msg_errno(redirect->filename, shell);
	return (exit_code);
}

/**
 * @brief Handles output redirection.
 *
 * This function opens a file for writing (creating or appending), 
 * saves the current stdout, redirects stdout to the opened file, 
 * executes the command, restores the original stdout, and closes 
 * the file. 
 * If the file cannot be opened, it returns an error code.
 *
 * @param redirect The redirection structure containing file and command 
 * information.
 * @param shell Pointer to the shell structure for process management.
 * @return The exit code of the command or an error code.
 */
int	process_output_redirection(t_redir *redirect, t_shell *shell)
{
	int		fd;
	int		exit_code;
	int		stdout_temp;
	bool	success;

	if (redirect->redir_type == T_OUTPUT)
		fd = open(redirect->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = open(redirect->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	success = (fd != -1);
	if (success)
	{
		stdout_temp = dup(STDOUT_FILENO);
		success = (duplicate_fd(fd, STDOUT_FILENO, shell) != -1);
		close_file(fd, shell);
	}
	if (success)
	{
		exit_code = run_cmd(redirect->command, OP_COMPLETE, shell);
		duplicate_fd(stdout_temp, STDOUT_FILENO, shell);
		close_file(stdout_temp, shell);
	}
	else
		exit_code = error_msg_errno(redirect->filename, shell);
	return (exit_code);
}

/**
 * @brief Handles input or output redirection.
 *
 * This function determines the type of redirection (input or output) 
 * and calls the appropriate function to handle the redirection.
 *
 * @param redirect The redirection structure containing file and command 
 * information.
 * @param shell Pointer to the shell structure for process management.
 * @return The exit code of the command or an error code.
 */
int	process_redirection(t_redir *redirect, t_shell *shell)
{
	if (redirect->redir_type == T_INPUT || redirect->redir_type == T_HDOC)
		return (process_input_redirection(redirect, shell));
	return (process_output_redirection(redirect, shell));
}
