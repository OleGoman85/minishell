/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_ev.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:26:57 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:06:47 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Displays the environment variable list.
 *
 * This function iterates through the environment variable list and prints 
 * each environment variable.
 * If `export_mode` is true, it prints variables in the format used by the 
 * `export` command (`declare -x`).
 * If `export_mode` is false, it prints only those variables that have a 
 non-null value.
 *
 * @param export_mode Boolean flag indicating whether to use export mode 
 (with `declare -x`) or not.
 * @param ev Pointer to the list of environment variables to be displayed.
 * @param shell Pointer to the shell structure for managing memory and 
 * file descriptors.
 * @return Returns `EXIT_SUCCESS` on completion.
 */
int	display_ev_list(bool export_mode, t_list *ev, t_shell *shell)
{
	while (ev != NULL)
	{
		if (!export_mode && !get_ev_value(ev))
		{
			ev = ev->next;
			continue ;
		}
		if (export_mode)
			write_and_track("declare -x ", STDOUT_FILENO, shell);
		write_and_track(get_ev_name(ev), STDOUT_FILENO, shell);
		if (get_ev_value(ev))
			write_and_track("=", STDOUT_FILENO, shell);
		if (export_mode && get_ev_value(ev))
		{
			write_and_track("\"", STDOUT_FILENO, shell);
			write_and_track(get_ev_value(ev), STDOUT_FILENO, shell);
			write_and_track("\"", STDOUT_FILENO, shell);
		}
		else if (get_ev_value(ev))
			write_and_track(get_ev_value(ev), STDOUT_FILENO, shell);
		write_and_track("\n", STDOUT_FILENO, shell);
		ev = ev->next;
	}
	return (EXIT_SUCCESS);
}
/**
 * @brief Fills an array with environment variable strings.
 *
 * This function populates a provided array with environment variable strings 
 * in the format `NAME=VALUE`.
 * It uses the environment variable list to construct the strings and tracks 
 * memory allocation.
 *
 * @param ev_array Array to be filled with environment variable strings.
 * @param ev_list Pointer to the list of environment variables.
 * @param shell Pointer to the shell structure for memory management.
 */

static void	fill_ev_array(char **ev_array, t_list *ev_list, t_shell *shell)
{
	int		i;
	char	*ev_value;
	char	*ev_name;

	i = 0;
	while (ev_list)
	{
		ev_value = get_ev_value(ev_list);
		if (ev_value)
		{
			ev_name = strjoin_tracked(get_ev_name(ev_list), "=", COMMAND_TRACK,
					shell);
			ev_array[i] = strjoin_tracked(ev_name, ev_value, COMMAND_TRACK,
					shell);
			i++;
		}
		ev_list = ev_list->next;
	}
	ev_array[i] = NULL;
}

char	**create_ev_array(t_shell *shell)
{
	int		ev_count;
	char	**env_array;

	ev_count = ft_lstsize(shell->ev_list);
	env_array = calloc_tracked(ev_count + 1, sizeof(char *), COMMAND_TRACK,
			shell);
	if (!env_array)
		return (NULL);
	fill_ev_array(env_array, shell->ev_list, shell);
	return (env_array);
}
