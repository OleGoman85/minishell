/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:56:59 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 06:31:12 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates or adds an environment variable.
 *
 * This function checks if the environment variable exists. If it does,
 * it updates its value. Otherwise, it adds the variable with the given value.
 *
 * @param ev Pointer to the environment variable node.
 * @param value The value to set for the environment variable.
 * @param shell Pointer to the shell structure.
 */
static void	update_env_var(t_list *ev, char *value, t_shell *shell)
{
	if (ev)
		change_ev_val(ev, value, false, shell);
	else
		add_ev(value, value, &(shell->ev_list), shell);
}
/**
 * @brief Updates the PWD and OLDPWD environment variables.
 *
 * This function updates the PWD and OLDPWD environment
 * variables using
 * the current working directory. If the current directory is not accessible,
 * it handles the corresponding errors.
 *
 * @param current_pwd Pointer to the string containing the current
 * working directory.
 * @param oldpwd_val The value of the OLDPWD environment variable.
 * @param shell Pointer to the shell structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */

static int	update_env_vars(char **current_pwd, char
		*oldpwd_val, t_shell *shell)
{
	char	*buffer;

	buffer = calloc_tracked(PATH_MAX, 1, COMMAND_TRACK, shell);
	if (!buffer)
		exit_on_sys_error("calloc_tracked: ", errno, shell);
	*current_pwd = getcwd(buffer, PATH_MAX);
	if (!*current_pwd)
	{
		if (errno == ENOENT)
		{
			error_msg("cd: error retrieving current directory: ",
				"getcwd: cannot access parent directories: ",
				strerror(errno), shell);
			change_ev_val(get_ev("PWD", shell->ev_list), "/.", true, shell);
		}
		else
			exit_on_sys_error("getcwd: ", errno, shell);
		return (EXIT_FAILURE);
	}
	update_env_var(get_ev("OLDPWD", shell->ev_list), oldpwd_val, shell);
	update_env_var(get_ev("PWD", shell->ev_list), *current_pwd, shell);
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
