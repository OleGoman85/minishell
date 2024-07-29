/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:36:36 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:01:39 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the array of built-in commands.
 *
 * This function fills the `commands` array with built-in command names and
 * their corresponding functions. The array is terminated with an element where
 * both the command name and function pointer are NULL.
 *
 * @param commands Pointer to the array of command structures to be filled
 * with built-in commands and their functions.
 */
void	init_builtins(t_bltn_cmd *commands)
{
	commands[0].cmd_name = "echo";
	commands[0].cmd_func = ft_echo;
	commands[1].cmd_name = "cd";
	commands[1].cmd_func = ft_cd;
	commands[2].cmd_name = "pwd";
	commands[2].cmd_func = ft_pwd;
	commands[3].cmd_name = "export";
	commands[3].cmd_func = ft_export;
	commands[4].cmd_name = "unset";
	commands[4].cmd_func = ft_unset;
	commands[5].cmd_name = "env";
	commands[5].cmd_func = ft_env;
	commands[6].cmd_name = "exit";
	commands[6].cmd_func = ft_exit;
	commands[7].cmd_name = NULL;
	commands[7].cmd_func = NULL;
}

/**
 * @brief Finds and returns the function of a built-in command by
 * its name.
 *
 * This function searches for a built-in command by its name in the
 * commands array.
 * If the command is found, it returns a pointer to the corresponding
 * function.
 * If the command is not found, it returns NULL.
 *
 * @param command The name of the command to find.
 * @return Pointer to the function of the built-in command, or NULL if
 * the command
 * is not found.
 */
t_bltn_func	fetch_builtin_cmd(char *command)
{
	t_bltn_cmd	commands[8];
	int			index;
	int			name_found;
	t_bltn_func	result_func;

	index = 0;
	name_found = 0;
	result_func = NULL;
	init_builtins(commands);
	while (commands[index].cmd_name != NULL && !name_found)
	{
		if (ft_strcmp(command, commands[index].cmd_name) == 0)
		{
			result_func = commands[index].cmd_func;
			name_found = 1;
		}
		index++;
	}
	return (result_func);
}
