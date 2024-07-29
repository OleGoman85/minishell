/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:20:08 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:03:31 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * check_var_name - Checks if a given string is a valid variable name.
 *
 * @param char *str: The string to be checked.
 *
 * This function validates whether the provided string conforms to the 
 * rules for variable names:
 * - It must start with an alphabetic character or an underscore ('_').
 * - The subsequent characters can be alphanumeric or underscores.
 *
 * @return bool: Returns `true` if the string is a valid variable 
 * name, `false` otherwise.
 */
static bool	check_var_name(char *str)
{
	char	*current_char;

	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	current_char = str + 1;
	while (*current_char != '\0')
	{
		if (!(ft_isalnum(*current_char) || *current_char == '_'))
			return (false);
		current_char++;
	}
	return (true);
}
/**
 * extract_var_name - Extracts the variable name from an environment 
 * variable string.
 *
 * @param char *ev_str: The environment variable string (e.g., "VAR=value").
 * @param char *equals_pos: Pointer to the '=' character in the environment
 *  variable string.
 * @param bool append_mode: Indicates if the variable is being appended to 
 * (i.e., using "+=").
 * @param t_shell *shell: Pointer to the shell structure for memory management.
 *
 * This function extracts the variable name part from the environment variable 
 * string.
 * It considers whether the string includes an append mode and adjusts the 
 * length accordingly.
 *
 * @return char*: The extracted variable name. Memory is managed and tracked.
 */

static char	*extract_var_name(char *ev_str, char *equals_pos, bool append_mode,
		t_shell *shell)
{
	int		name_length;
	char	*var_name;

	name_length = equals_pos - append_mode - ev_str;
	var_name = ft_substr(ev_str, 0, name_length);
	return (manage_memory(var_name, COMMAND_TRACK, shell));
}
/**
 * process_export - Processes and updates or adds an environment variable.
 *
 * @param char *ev_str: The environment variable string (e.g., "VAR=value").
 * @param bool *has_invalid_name: Pointer to a flag indicating if any variable
 *  names were invalid.
 * @param t_shell *shell: Pointer to the shell structure.
 *
 * This function handles the export of an environment variable. It performs the
 *  following:
 * 1. Extracts the variable name and determines if it is valid.
 * 2. Updates the environment variable if it already exists, or adds it if it 
 * does not.
 * 3. Checks if the variable name is valid and sets an error flag if it is not.
 */

static void	process_export(char *ev_str, bool *has_invalid_name, t_shell *shell)
{
	char	*ev_name;
	char	*equals_pos;
	t_list	*ev_node;
	bool	append_mode;

	equals_pos = ft_strchr(ev_str, '=');
	append_mode = (ft_strnstr(ev_str, "+=", equals_pos - ev_str + 1) != NULL);
	ev_name = extract_var_name(ev_str, equals_pos, append_mode, shell);
	if (!check_var_name(ev_name))
	{
		error_msg("export: `", ev_str, "': not a valid identifier", shell);
		*has_invalid_name = true;
		return ;
	}
	ev_node = get_ev(ev_name, shell->ev_list);
	if (ev_node && equals_pos)
		change_ev_val(ev_node, equals_pos + 1, append_mode, shell);
	else if (!ev_node)
	{
		if (equals_pos)
			add_ev(ev_name, equals_pos + 1, &(shell->ev_list), shell);
		else
			add_ev(ev_name, NULL, &(shell->ev_list), shell);
	}
}
/**
 * ft_export - Handles the export command to update or add environment 
 * variables.
 *
 * @param t_cmd *cmd: Pointer to the command structure containing the 
 * arguments.
 * @param t_shell *shell: Pointer to the shell structure containing 
 * environment variables.
 *
 * This function processes each argument passed to the `export` command. 
 * It validates variable names,
 * updates existing environment variables, or adds new ones. If any invalid 
 * variable names are found,
 * it returns an error code.
 *
 * @return int: Returns `EXIT_SUCCESS` if all variables are processed 
 * successfully, `EXIT_FAILURE`
 * if any variable names are invalid.
 */

int	ft_export(t_cmd *cmd, t_shell *shell)
{
	bool	has_invalid_name;
	char	**args;

	args = cmd->cmd_args + 1;
	if (!*args)
		return (display_ev_list(true, shell->ev_list, shell));
	has_invalid_name = false;
	while (*args)
	{
		process_export(*args, &has_invalid_name, shell);
		args++;
	}
	if (has_invalid_name)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
