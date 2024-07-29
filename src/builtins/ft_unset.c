/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:13:13 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:04:02 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	char	**args;
	t_list	*ev;

	args = cmd->cmd_args + 1;
	while (*args != NULL)
	{
		ev = get_ev(*args, shell->ev_list);
		if (ev != NULL)
			del_node(&ev, &(shell->ev_list), free_ev, true);
		args++;
	}
	return (EXIT_SUCCESS);
}
/**
 * ft_unset - Removes specified environment variables from 
 *     the shell's environment.
 *
 * @param t_cmd *cmd: Pointer to the command structure containing 
 * the command and its arguments.
 * @param t_shell *shell: Pointer to the shell structure representing 
 * the current state of the shell, including the environment variable list.
 *
 * This function processes the `unset` command. It iterates over each 
 * argument provided (each representing an environment 
 * variable name to be unset),
 * finds the corresponding environment variable in the shell's environment 
 * variable list, and removes it if it exists.
 * 
 * Steps:
 * 1. Extract Arguments:
 *    - Retrieve the command arguments, skipping the first argument 
 * (which is the command itself, typically "unset").
 * 
 * 2. Iterate Through Arguments:
 *    - Loop through each argument provided to the `unset` command.
 * 
 * 3. Find Environment Variable:
 *    - For each argument, use the `get_ev` function to locate the 
 * corresponding environment variable in the shell's environment list.
 * 
 * 4. Delete Environment Variable:
 *    - If the environment variable is found (`ev` is not `NULL`), 
 * call the `del_node` function to remove the environment variable 
 * node from the list and free its associated memory.
 * 
 * 5. Continue Until All Arguments Processed:
 *    - Repeat the process for all provided environment variable names.
 * 
 * 6. Return Success:
 *    - Return `EXIT_SUCCESS`, indicating that the `unset` operation 
 * completed successfully.
 * 
 * @return int: Returns `EXIT_SUCCESS` to indicate success.
 */