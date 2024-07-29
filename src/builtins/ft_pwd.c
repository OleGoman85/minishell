/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:19:07 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:03:41 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_cmd *cmd, t_shell *shell)
{
	t_list	*pwd_ev;
	char	*pwd_value;
	char	*dir_path;

	(void)cmd;
	pwd_ev = get_ev("PWD", shell->ev_list);
	pwd_value = get_ev_value(pwd_ev);
	if (pwd_value)
		write_and_track(pwd_value, STDOUT_FILENO, shell);
	else
	{
		dir_path = getcwd(NULL, 0);
		if (!dir_path)
		{
			exit_on_sys_error("getcwd", errno, shell);
			return (EXIT_FAILURE);
		}
		manage_memory(dir_path, COMMAND_TRACK, shell);
		write_and_track(dir_path, STDOUT_FILENO, shell);
	}
	write_and_track("\n", STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}

/**
 * ft_pwd - Prints the current working directory.
 *
 * @param t_cmd *cmd: Pointer to the command structure containing 
 * the command and its arguments (not used in this function).
 * @param t_shell *shell: Pointer to the shell structure representing 
 * the current state of the shell, including the environment variable list.
 *
 * This function processes the `pwd` command, which prints the 
 * current working directory.
 * 
 * Steps:
 * 1. Get the "PWD" Environment Variable:
 *    - Retrieve the "PWD" environment variable from the shell's 
 * environment list using the `get_ev` function.
 * 
 * 2. Get the Value of "PWD":
 *    - Extract the value of the "PWD" environment variable 
 * using the `get_ev_value` function.
 * 
 * 3. Print "PWD" Value if Available:
 *    - If the "PWD" value is available, print it to the standard 
 * output using the `write_and_track` function.
 * 
 * 4. Get and Print Current Directory if "PWD" Not Available:
 *    - If the "PWD" value is not available, use `getcwd` to get the 
 * current working directory.
 *    - If `getcwd` fails, handle the error using `exit_on_sys_error` 
 * and return `EXIT_FAILURE`.
 *    - If successful, manage the memory for the directory path using 
 * `manage_memory` and print it to the standard output.
 * 
 * 5. Print Newline:
 *    - Print a newline character to the standard output using 
 * `write_and_track`.
 * 
 * 6. Return Success:
 *    - Return `EXIT_SUCCESS`, indicating that the `pwd` operation 
 * completed successfully.
 * 
 * @return int: Returns `EXIT_SUCCESS` to indicate success, or 
 * `EXIT_FAILURE` in case of an error.
 */