/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:02:52 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 08:08:47 by ogoman           ###   ########.fr       */
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

/**
 * @brief Opens a directory and handles errors.
 *
 * This function tries to open a directory at the specified path and checks 
 * for errors.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param path Path to the directory to open.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the `DIR` structure representing the opened directory.
 */
DIR	*open_directory(const char *path, t_shell *shell)
{
	DIR	*directory;

	errno = 0;
	directory = opendir(path);
	if (!directory)
	{
		if (errno != 0)
			exit_on_sys_error("opendir failed", errno, shell);
	}
	return (directory);
}

/**
 * @brief Closes a directory and handles errors.
 *
 * This function tries to close a directory given by its `DIR` 
 * pointer and checks for errors.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param directory Pointer to the `DIR` structure representing the 
 * directory to close.
 * @param shell Pointer to the shell structure for memory management.
 * @return Status of the directory close operation. 0 on success, 
 * -1 on error.
 */
int	close_directory(DIR *directory, t_shell *shell)
{
	int		close_status;
	bool	error_occurred;

	errno = 0;
	close_status = closedir(directory);
	error_occurred = (close_status == -1 && errno != 0);
	if (error_occurred)
		exit_on_sys_error("failed to close directory", errno, shell);
	return (close_status);
}

/**
 * @brief Reads an entry from a directory and handles errors.
 *
 * This function tries to read the next entry from the given directory
 * and checks for errors.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param directory Pointer to the `DIR` structure representing the 
 * directory.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the `dirent` structure representing the directory 
 * entry.
 */
struct dirent	*read_directory(DIR *directory, t_shell *shell)
{
	struct dirent	*entry;
	bool			error_occurred;

	errno = 0;
	entry = readdir(directory);
	error_occurred = (entry == NULL && errno != 0);
	if (error_occurred)
		exit_on_sys_error("failed to read directory entry", errno, shell);
	return (entry);
}

/**
 * @brief Opens a file with specified flags and permissions.
 *
 * This function opens a file at the specified path with the given flags 
 * and permissions.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param path Path to the file.
 * @param mode_flags Flags for opening the file.
 * @param permissions Permissions for creating the file.
 * @param shell Pointer to the shell structure for process management.
 * @return File descriptor of the opened file.
 */
int	open_file(const char *path, int mode_flags, mode_t permissions,
		t_shell *shell)
{
	int		file_descriptor;
	bool	error_occurred;

	file_descriptor = open(path, mode_flags, permissions);
	error_occurred = (file_descriptor == -1);
	if (error_occurred)
		exit_on_sys_error("open failed", errno, shell);
	return (file_descriptor);
}

/**
 * @brief Closes a file.
 *
 * This function closes a file given by its file descriptor.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param fd File descriptor of the file to close.
 * @param shell Pointer to the shell structure for process management.
 * @return Status of the file close operation. 0 on success, -1 on error.
 */
int	close_file(int fd, t_shell *shell)
{
	int		close_status;
	bool	error_occurred;

	close_status = close(fd);
	error_occurred = (close_status == -1);
	if (error_occurred)
		exit_on_sys_error("close failed", errno, shell);
	return (close_status);
}

/**
 * @brief Gets the status of a file and handles errors.
 *
 * This function obtains the status of a file specified by its path and fills
 * the `stat` structure with information about the file. If an error occurs,
 * it terminates the program with an error message.
 *
 * @param file_path Path to the file.
 * @param file_stat Pointer to a `stat` structure to store file information.
 * @param shell Pointer to the shell structure for process management.
 * @return Status of the `stat` operation. 0 on success, -1 on error.
 */
int	get_file_status(const char *restrict file_path,
		struct stat *restrict file_stat, t_shell *shell)
{
	int		stat_status;
	bool	error_occurred;

	stat_status = stat(file_path, file_stat);
	error_occurred = (stat_status == -1);
	if (error_occurred)
		exit_on_sys_error("stat failed", errno, shell);
	return (stat_status);
}

/**
 * @brief Duplicates a file descriptor.
 *
 * This function duplicates `source_fd` to `target_fd`.
 * If an error occurs, it terminates the program with an error message.
 *
 * @param source_fd Source file descriptor.
 * @param target_fd Target file descriptor.
 * @param shell Pointer to the shell structure for process management.
 * @return Status of the `dup2` operation. 0 on success, -1 on error.
 */
int	duplicate_fd(int source_fd, int target_fd, t_shell *shell)
{
	int		dup_status;
	bool	error_occurred;

	dup_status = dup2(source_fd, target_fd);
	error_occurred = (dup_status == -1);
	if (error_occurred)
		exit_on_sys_error("dup2 failed", errno, shell);
	return (dup_status);
}
