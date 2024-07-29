/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_second.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:53:50 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:27:05 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds a new string to an array of strings.
 *
 * This function first counts the number of strings in the existing array,
 * then creates a new array with space for one additional string,
 * copies the existing strings into the new array, and adds the new string.
 *
 * @param args Pointer to the array of strings.
 * @param str New string to be added.
 * @param shell Pointer to the shell structure for memory management.
 */
void	append_str(char ***args, char *str, t_shell *shell)
{
	int		original_count;
	int		i;
	char	**res_array;

	original_count = 0;
	i = 0;
	if (args && *args)
	{
		while ((*args)[original_count] != NULL)
			original_count++;
	}
	res_array = calloc_tracked(original_count + 2, sizeof(char *),
			COMMAND_TRACK, shell);
	while (i < original_count)
	{
		res_array[i] = (*args)[i];
		i++;
	}
	res_array[i] = str;
	res_array[i + 1] = NULL;
	*args = res_array;
}

/**
 * @brief Waits for a child process to terminate and handles errors.
 *
 * This function uses the `wait` system call to wait for a child process
 * to terminate.
 * In case of an error (i.e., `wait` returns -1), it calls `exit_on_sys_error` 
 * to print an error message and terminate the program.
 *
 * @param status Pointer to an integer where the exit status of the child
 * process will be stored.
 * @param shell Pointer to the shell structure for process management.
 * @return The PID of the terminated child process, or terminates the program
 * with an error message.
 */
pid_t	wait_for_child(int *status, t_shell *shell)
{
	pid_t	child_pid;
	bool	error_occurred;

	child_pid = wait(status);
	error_occurred = (child_pid == -1);
	if (error_occurred)
		exit_on_sys_error("wait failed", errno, shell);
	return (child_pid);
}

/**
 * @brief Creates a pipe and handles errors.
 *
 * This function uses the `pipe` system call to create a pipe. If an
 * error occurs 
 * (i.e., `pipe` returns -1), it calls `exit_on_sys_error` to print an 
 * error message 
 * and terminate the program.
 *
 * @param pipe_fds Array of two integers to hold the pipe file descriptors.
 * @param shell Pointer to the shell structure for process management.
 * @return 0 on success, otherwise terminates the program with an error 
 * message.
 */
int	create_pipe(int pipe_fds[2], t_shell *shell)
{
	int		pipe_status;
	bool	error_occurred;

	pipe_status = pipe(pipe_fds);
	error_occurred = (pipe_status == -1);
	if (error_occurred)
		exit_on_sys_error("pipe failed", errno, shell);
	return (pipe_status);
}

/**
 * @brief Executes a new program, replacing the current process.
 *
 * This function uses the `execve` system call to execute a new program,
 * replacing
 * the current process. If `execve` fails (returns -1), it calls 
 * `exit_on_sys_error`
 * to print an error message and terminate the program.
 *
 * @param path Path to the executable file.
 * @param args Array of arguments for the executable file.
 * @param env Array of environment variables for the new process.
 * @param shell Pointer to the shell structure for process management.
 * @return This function does not return on success as the current process 
 * is replaced.
 *         On failure, it returns -1.
 */
int	execute_program(const char *path, char *const args[], char *const env[],
		t_shell *shell)
{
	int		exec_status;
	bool	error_occurred;

	exec_status = execve(path, args, env);
	error_occurred = (exec_status == -1);
	if (error_occurred)
		exit_on_sys_error("execve failed", errno, shell);
	return (exec_status);
}

/**
 * @brief Creates a new process using `fork` and handles errors.
 *
 * This function calls the `fork` system call to create a new process. 
 * If `fork`
 * fails (returns -1), it calls `exit_on_sys_error` to print an error 
 * message
 * and terminate the program.
 *
 * @param shell Pointer to the shell structure for process management.
 * @return PID of the new process on success, otherwise terminates the 
 * program with an error message.
 */
pid_t	create_process(t_shell *shell)
{
	pid_t	process_id;
	bool	error_occurred;

	process_id = fork();
	error_occurred = (process_id == -1);
	if (error_occurred)
		exit_on_sys_error("fork failed", errno, shell);
	return (process_id);
}
