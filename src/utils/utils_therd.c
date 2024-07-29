/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_therd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:57:21 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:27:12 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
