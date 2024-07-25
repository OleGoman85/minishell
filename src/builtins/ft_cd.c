/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:47:45 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 09:48:29 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// /**
//  * @brief Updates the environment variables PWD and OLDPWD.
//  *
//  * This function updates the environment variables PWD (current working
//  * directory)
//  * and OLDPWD (previous working directory) based on the current directory.
//  * If the current directory cannot be accessed, it sets PWD to "/." and handles
//  * errors.
//  * "cd: error retrieving current directory: ", "getcwd: cannot access parent
//  * directories: " == "cd: error retr cur dir ", "getcwd: can't ac par dir: "
//  *
//  * @param current_pwd Pointer to a string where the current working directory
//  * will be stored.
//  * @param oldpwd_val Value of the OLDPWD environment variable before changing
//  * directories.
//  * @param shell Pointer to the shell structure for managing environment
//  * variables.
//  * @return Exit code (EXIT_SUCCESS on success, EXIT_FAILURE on error).
//  */
// static int	update_env_vars(char **current_pwd, char *oldpwd_val,
// 		t_shell *shell)
// {
// 	char	*buffer;
// 	t_list	*ev;

// 	buffer = calloc_tracked(PATH_MAX, 1, COMMAND_TRACK, shell);
// 	if (!buffer)
// 		exit_on_sys_error("calloc_tracked: ", errno, shell);
// 	if (!(*current_pwd = getcwd(buffer, PATH_MAX)))
// 	{
// 		if (errno == ENOENT)
// 		{
// 			error_msg("cd: error retr cur dir ", "getcwd: can't ac par dir: ",
// 				strerror(errno), shell);
// 			change_ev_val(get_ev("PWD", shell->ev_list), "/.", true, shell);
// 		}
// 		else
// 			exit_on_sys_error("getcwd: ", errno, shell);
// 		return (EXIT_FAILURE);
// 	}
// 	if ((ev = get_ev("OLDPWD", shell->ev_list)))
// 		change_ev_val(ev, oldpwd_val, false, shell);
// 	else
// 		add_ev("OLDPWD", oldpwd_val, &(shell->ev_list), shell);
// 	if ((ev = get_ev("PWD", shell->ev_list)))
// 		change_ev_val(ev, *current_pwd, false, shell);
// 	return (EXIT_SUCCESS);
// }

static int	handle_getcwd_error(char *buffer, t_shell *shell)
{
	(void)*buffer;
	if (errno == ENOENT)
	{
		error_msg("cd: error retrieving current directory",
			"getcwd: cannot access parent directories: ",
			strerror(errno), shell);
		change_ev_val(get_ev("PWD", shell->ev_list), "/.", true, shell);
	}
	else
		exit_on_sys_error("getcwd failed", errno, shell);
	return (EXIT_FAILURE);
}

static int	update_current_directory(char **current_pwd, t_shell *shell)
{
	char	*buffer;

	buffer = calloc_tracked(PATH_MAX, 1, COMMAND_TRACK, shell);
	if (!buffer)
		exit_on_sys_error("calloc_tracked failed", errno, shell);
	*current_pwd = getcwd(buffer, PATH_MAX);
	if (*current_pwd == NULL)
	{
		return (handle_getcwd_error(buffer, shell));
	}
	return (EXIT_SUCCESS);
}

static void	update_env_vars_internal(char *current_pwd, char *oldpwd_val,
		t_shell *shell)
{
	t_list	*ev;

	ev = get_ev("OLDPWD", shell->ev_list);
	if (ev != NULL)
		change_ev_val(ev, oldpwd_val, false, shell);
	else
		add_ev("OLDPWD", oldpwd_val, &(shell->ev_list), shell);
	ev = get_ev("PWD", shell->ev_list);
	if (ev != NULL)
		change_ev_val(ev, current_pwd, false, shell);
}
/**
 * @brief Updates the PWD and OLDPWD environment variables.
 *
 * This function updates the environment variables PWD (current
 * working directory)
 * and OLDPWD (previous working directory). It retrieves the current
 * directory,
 * handles errors if the directory cannot be accessed, and updates the
 * environment
 * variables accordingly.
 *
 * @param current_pwd Pointer to a string that will hold the current working 
 * directory.
 * @param oldpwd_val Value of the OLDPWD environment variable before 
 * changing directories.
 * @param shell Pointer to the shell structure for managing environment 
 * variables.
 * @return Exit code (EXIT_SUCCESS on success, EXIT_FAILURE on error).
 */

static int	update_env_vars(char **current_pwd, char *oldpwd_val,
		t_shell *shell)
{
	int	result;

	result = update_current_directory(current_pwd, shell);
	if (result != EXIT_SUCCESS)
		return (result);
	update_env_vars_internal(*current_pwd, oldpwd_val, shell);
	return (EXIT_SUCCESS);
}

/**
 * @brief Processes arguments for the cd command to determine the target
 * directory.
 *
 * This function processes the arguments provided to the cd command to
 * determine
 * which directory to change to. It handles cases for no arguments, "-"
 * (to go to
 * the previous directory), and a specific directory path.
 *
 * @param cmd Pointer to the command structure containing the arguments
 * for cd.
 * @param target_dir Pointer to a string where the target directory will
 * be set.
 * @param shell Pointer to the shell structure for managing environment
 * variables.
 * @return Exit code (EXIT_SUCCESS on success, EXIT_FAILURE on error).
 */
static int	process_cd_args(t_cmd *cmd, char **target_dir, t_shell *shell)
{
	t_list	*env_var;

	if (cmd->cmd_args[1] == NULL)
	{
		env_var = get_ev("HOME", shell->ev_list);
		if (env_var == NULL)
			return (error_msg("cd: ", NULL,
					"HOME environment variable is not set", shell));
		*target_dir = get_ev_value(env_var);
	}
	else if (ft_strcmp(cmd->cmd_args[1], "") == 0)
		*target_dir = NULL;
	else if (ft_strcmp(cmd->cmd_args[1], "-") == 0)
	{
		env_var = get_ev("OLDPWD", shell->ev_list);
		if (env_var == NULL)
			return (error_msg("cd: ", NULL,
					"OLDPWD environment variable is not set", shell));
		*target_dir = strdup_tracked(get_ev_value(env_var),
				COMMAND_TRACK, shell);
	}
	else
		*target_dir = cmd->cmd_args[1];
	return (EXIT_SUCCESS);
}

/**
 * @brief Changes the current working directory.
 *
 * This function executes the cd command to change the current
 * working directory.
 * It checks for errors in arguments, changes the directory, and updates
 * the environment variables accordingly. It also handles printing the previous
 * directory if the "-" argument is used.
 *
 * @param cmd Pointer to the command structure containing the arguments for cd.
 * @param shell Pointer to the shell structure for managing environment
 * variables.
 * @return Exit code (EXIT_SUCCESS on success, EXIT_FAILURE on error).
 */
int	ft_cd(t_cmd *cmd, t_shell *shell)
{
	char	*current_pwd;
	char	*target_dir;

	current_pwd = NULL;
	target_dir = NULL;
	if (cmd->cmd_args[1] != NULL && cmd->cmd_args[2] != NULL)
		return (error_msg("cd: ", NULL, "too many arguments", shell));
	if (process_cd_args(cmd, &target_dir, shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (target_dir != NULL && chdir(target_dir) == -1)
		return (error_msg("cd: ", target_dir, strjoin_tracked(": ",
					strerror(errno), COMMAND_TRACK, shell), shell));
	if (update_env_vars(&current_pwd,
			get_ev_value(get_ev("PWD", shell->ev_list)), shell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (cmd->cmd_args[1] && ft_strcmp(cmd->cmd_args[1], "-") == 0)
		write_and_track(strjoin_tracked(current_pwd, "\n", COMMAND_TRACK,
				shell), STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}
