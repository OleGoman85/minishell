/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:02:52 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:27:19 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
