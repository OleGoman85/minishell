/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:32:19 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/24 07:32:47 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * ft_env - Handles the `env` command to display environment variables.
 *
 * @param t_cmd *cmd: Pointer to the command structure containing 
 * the arguments.
 * @param t_shell *shell: Pointer to the shell structure containing 
 * environment variables.
 *
 * This function processes the `env` command, which is used to display 
 * the current environment variables.
 * It performs the following tasks:
 * 1. Counts the number of arguments passed to the command.
 * 2. Checks if there are more than one argument. If so, it reports an 
 * error because `env` expects no more than one argument.
 * 3. If the argument count is valid (0 or 1), it proceeds to display 
 * the environment variables.
 *
 * @return int: Returns `EXIT_SUCCESS` if the environment variables are 
 * displayed successfully, or `EXIT_FAILURE`
 * if there are more arguments than expected.
 */
int	ft_env(t_cmd *cmd, t_shell *shell)
{
	int	arg_count;

	arg_count = 0;
	if (cmd->cmd_args)
	{
		while (cmd->cmd_args[arg_count])
			arg_count++;
	}
	if (arg_count > 1)
	{
		error_msg("env: ", NULL, "more arguments than expected", shell);
		return (EXIT_FAILURE);
	}
	display_ev_list(false, shell->ev_list, shell);
	return (EXIT_SUCCESS);
}
