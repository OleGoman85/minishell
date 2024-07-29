/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_second.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 12:14:32 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:07:05 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a list of environment variables from an array of strings.
 *
 * This function initializes a list of environment variables from an array 
 * of strings in the format `NAME=VALUE`.
 * It also sets a default `PATH` if not already present and extracts the 
 * `HOME` directory to set in the shell structure.
 *
 * @param env_vars Array of environment variable strings.
 * @param shell Pointer to the shell structure for managing memory.
 * @return Pointer to the list of environment variables.
 */
t_list	*create_ev_list(char **env_vars, t_shell *shell)
{
	char	*equals_ptr;
	char	*name;
	char	*extracted_name;
	t_list	*ev_list;
	char	*home_val;

	ev_list = NULL;
	while (*env_vars)
	{
		equals_ptr = ft_strchr(*env_vars, '=');
		extracted_name = ft_substr(*env_vars, 0, equals_ptr - *env_vars);
		name = manage_memory(extracted_name, COMMAND_TRACK, shell);
		add_ev(name, equals_ptr + 1, &(ev_list), shell);
		env_vars++;
	}
	if (!get_ev("PATH", ev_list))
		add_ev("PATH", DEFAULT_PATH, &(ev_list), shell);
	home_val = get_ev_value(get_ev("HOME", ev_list));
	if (home_val)
		shell->home_dir = strdup_tracked(home_val, CORE_TRACK, shell);
	else
		shell->home_dir = NULL;
	return (ev_list);
}
/**
 * @brief Adds a new environment variable to the list.
 *
 * This function creates a new environment variable structure and adds it to 
 * the end of the environment variable list.
 *
 * @param name Name of the environment variable.
 * @param value Value of the environment variable.
 * @param ev_list Pointer to the list of environment variables.
 * @param shell Pointer to the shell structure for managing memory.
 */

void	add_ev(char *name, char *value, t_list **ev_list, t_shell *shell)
{
	t_env_var	*ev;

	ev = calloc_tracked(1, sizeof(t_env_var), UNTRACKED, shell);
	ev->name = strdup_tracked(name, UNTRACKED, shell);
	ev->value = NULL;
	if (value)
		ev->value = strdup_tracked(value, UNTRACKED, shell);
	lstadd_back_tracked(ev, ev_list, UNTRACKED, shell);
}
/**
 * @brief Changes the value of an existing environment variable.
 *
 * This function updates the value of an environment variable in the list. It 
 * can either replace the old value
 * or append the new value to the existing one based on the `retain_old` flag.
 *
 * @param ev_ptr Pointer to the environment variable list node.
 * @param new_val New value to set for the environment variable.
 * @param retain_old Flag indicating whether to append the new value to the 
 * old one.
 * @param shell Pointer to the shell structure for managing memory.
 */

void	change_ev_val(t_list *ev_ptr, char *new_val,
						bool retain_old, t_shell *shell)
{
	t_env_var	*ev;
	char		*old_val;

	ev = (t_env_var *)(ev_ptr->content);
	old_val = ev->value;
	if (!new_val)
		ev->value = NULL;
	else if (retain_old)
		ev->value = strjoin_tracked(ev->value, new_val, UNTRACKED, shell);
	else
		ev->value = strdup_tracked(new_val, UNTRACKED, shell);
	free(old_val);
}

/**
 * @brief Retrieves the environment variable list node with a specified name.
 *
 * This function searches the environment variable list for a node with 
 * the given name.
 *
 * @param target Name of the environment variable to find.
 * @param ev_list Pointer to the list of environment variables.
 * @return Pointer to the environment variable list node if found, 
 * otherwise NULL.
 */
t_list	*get_ev(char *target, t_list *ev_list)
{
	t_env_var	*current_ev;

	while (ev_list)
	{
		current_ev = (t_env_var *)(ev_list->content);
		if (ft_strcmp(target, current_ev->name) == 0)
			return (ev_list);
		ev_list = ev_list->next;
	}
	return (NULL);
}

/**
 * @brief Retrieves the name of an environment variable from a list node.
 *
 * This function gets the name of the environment variable from the 
 * specified list node.
 *
 * @param ev_node Pointer to the environment variable list node.
 * @return Name of the environment variable, or NULL if the node is NULL.
 */
char	*get_ev_name(t_list *ev_node)
{
	if (!ev_node)
		return (NULL);
	return (((t_env_var *)(ev_node->content))->name);
}
