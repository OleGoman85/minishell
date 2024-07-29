/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 07:34:05 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:02:58 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Returns the replacement character for an escape sequence.
 *
 * This function takes a character representing an escape sequence and returns
 * the corresponding character. For example, '\n' is replaced with a newline
 * character, '\r' with a carriage return, and '\t' with a tab.
 *
 * @param c The character representing the escape sequence.
 * @return The replacement character for the escape sequence.
 */
static char	get_escape_replacement(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 'r')
		return ('\r');
	if (c == 't')
		return ('\t');
	return (c);
}

/**
 * @brief Processes escape sequences in a string.
 *
 * This function takes a string, processes all escape sequences in it, 
 * and returns
 * a new string with the characters replaced. Memory for the new string 
 * is allocated
 * using the `calloc_tracked` function.
 *
 * @param arg The input string to process.
 * @param shell Pointer to the shell data structure.
 * @return A new string with escape sequences replaced.
 */
static char	*process_escape_sequences(const char *arg, t_shell *shell)
{
	char	*result;
	size_t	len;
	int		i;
	int		j;

	len = ft_strlen(arg);
	i = 0;
	j = 0;
	result = calloc_tracked(len + 1, sizeof(char), COMMAND_TRACK, shell);
	if (!result)
		exit_on_sys_error("calloc_tracked: ", errno, shell);
	while (arg[i])
	{
		if (arg[i] == '\\' && arg[i + 1])
			result[j++] = get_escape_replacement(arg[++i]);
		else
			result[j++] = arg[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/**
 * @brief Checks for the presence of a flag among command arguments.
 *
 * This function checks if a flag (character) is present among the 
 * command arguments,
 * starting from a specified index. If the flag is found, the index 
 * is incremented,
 * and the function returns true.
 *
 * @param cmd_args Array of command arguments.
 * @param index Pointer to the index from which to start checking.
 * @param flag The flag character to look for.
 * @return true if the flag is found, otherwise false.
 */
static bool	has_flag(char **cmd_args, int *index, char flag)
{
	bool	flag_found;
	int		char_pos;

	flag_found = false;
	while (cmd_args[*index] && cmd_args[*index][0] == '-')
	{
		char_pos = 1;
		while (cmd_args[*index][char_pos] == flag)
			char_pos++;
		if (cmd_args[*index][char_pos] == '\0' && char_pos > 1)
		{
			flag_found = true;
			(*index)++;
		}
		else
			return (flag_found);
	}
	return (flag_found);
}

/**
 * @brief Executes the echo command with escape sequence processing.
 *
 * This function executes the echo command, processing escape 
 * sequences in the
 * command arguments and writing them to standard output. If the 
 * command arguments
 * contain the -n flag, it disables the newline output at the end.
 *
 * @param cmd Pointer to the command structure.
 * @param shell Pointer to the shell data structure.
 * @return EXIT_SUCCESS upon successful execution of the command.
 */
int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	bool	add_newline;
	int		i;
	char	*processed_arg;

	add_newline = true;
	i = 1;
	if (has_flag(cmd->cmd_args, &i, 'n'))
		add_newline = false;
	while (cmd->cmd_args[i])
	{
		processed_arg = process_escape_sequences(cmd->cmd_args[i], shell);
		write_and_track(processed_arg, STDOUT_FILENO, shell);
		if (cmd->cmd_args[i + 1])
			write_and_track(" ", STDOUT_FILENO, shell);
		i++;
	}
	if (add_newline)
		write_and_track("\n", STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}
