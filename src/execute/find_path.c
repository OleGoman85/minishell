/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:06:35 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:04:53 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Generates an array of paths from the PATH environment variable.
 *
 * This function retrieves the value of the PATH environment variable 
 * and splits it
 * into an array of strings representing different paths. If PATH includes the
 * current directory (".") as one of the paths, it is added to the 
 * array of paths.
 *
 * @param shell Pointer to the shell structure for memory management.
 * @return Array of strings containing paths from the PATH environment 
 * variable.
 */
char	**generate_paths(t_shell *shell)
{
	char	**dir_paths;
	char	*env_paths;
	bool	includes_dot;

	includes_dot = false;
	dir_paths = NULL;
	env_paths = get_ev_value(get_ev("PATH", shell->ev_list));
	if (!env_paths)
		env_paths = strdup_tracked(":", COMMAND_TRACK, shell);
	dir_paths = split_tracked(env_paths, ':', COMMAND_TRACK, shell);
	if (env_paths[0] == ':' || env_paths[ft_strlen(env_paths) - 1] == ':'
		|| ft_strnstr(env_paths, "::", ft_strlen(env_paths)) != NULL)
		includes_dot = true;
	if (includes_dot)
		append_str(&dir_paths, strdup_tracked(".", COMMAND_TRACK, shell),
			shell);
	return (dir_paths);
}

/**
 * @brief Finds the path to an executable file.
 *
 * This function checks whether the command is a given path or needs to be
 * searched in the PATH environment variable. If the command contains
 * '/', it checks the file's availability at the specified path.
 * Otherwise, the function searches for the command in PATH.
 *
 * @param cmd Command name or path to the executable file.
 * @param shell Pointer to the shell structure for memory management.
 * @return Path to the executable file or NULL if the command is not found.
 */
char	*find_executable_path(char *cmd, t_shell *shell)
{
	struct stat	file_info;

	if (ft_strncmp(cmd, "", 1) == 0)
		exit_on_error("command not found", cmd, 127, shell);
	else if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == -1)
		{
			if (errno == EACCES)
				exit_on_error("Permission denied", cmd, 126, shell);
			else if (errno == ENOENT)
				exit_on_error("No such file or directory", cmd, 127, shell);
			else
				exit_on_sys_error(cmd, errno, shell);
		}
		get_file_status(cmd, &file_info, shell);
		if (S_ISDIR(file_info.st_mode))
			exit_on_error("Is a directory", cmd, 126, shell);
		return (cmd);
	}
	else if (ft_strncmp(cmd, ".", 1) == 0 || ft_strncmp(cmd, "..", 2) == 0)
		exit_on_error("command not found", cmd, 127, shell);
	else
		return (locate_executable(cmd, shell));
	return (NULL);
}
/**
 * @brief Searches for an executable file in the directories 
 * specified in PATH.
 *
 * This function iterates over directories in the PATH variable 
 * and tries to find
 * the executable file. If the file is found and is executable, 
 * the function returns
 * its full path. Otherwise, it returns an error message.
 *
 * @param bin_name Name of the executable file.
 * @param sh Pointer to the shell structure for memory management.
 * @return Full path to the executable file or NULL if the command 
 * is not found.
 */

char	*locate_executable(char *bin_name, t_shell *sh)
{
	char	*full_path;
	bool	access_error;
	char	**dir_paths;
	int		i;

	dir_paths = generate_paths(sh);
	if (!dir_paths)
		exit_on_error("command not found in PATH", bin_name, 127, sh);
	access_error = false;
	i = -1;
	while (dir_paths[++i] != NULL)
	{
		full_path = strjoin_tracked(strjoin_tracked(dir_paths[i], "/",
					COMMAND_TRACK, sh), bin_name, COMMAND_TRACK, sh);
		if (access(full_path, X_OK) == EXIT_SUCCESS)
			return (full_path);
		else if (errno == EACCES)
			access_error = true;
		else if (errno != ENOENT)
			exit_on_sys_error("error accessing path", errno, sh);
	}
	if (access_error)
		exit_on_error("Permission denied", bin_name, 126, sh);
	exit_on_error("command not found", bin_name, 127, sh);
	return (NULL);
}
