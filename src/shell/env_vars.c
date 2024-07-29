/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:28:53 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:07:16 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Retrieves the value of an environment variable from a list node.
 *
 * This function gets the value of the environment variable from the
 *  specified list node.
 *
 * @param ev_node Pointer to the environment variable list node.
 * @return Value of the environment variable, or NULL if the node is NULL.
 */
char	*get_ev_value(t_list *ev_node)
{
	if (!ev_node)
		return (NULL);
	return (((t_env_var *)(ev_node->content))->value);
}

/**
 * @brief Frees the memory allocated for an environment variable.
 *
 * This function deallocates memory for the name and value of an environment 
 * variable and then frees the environment variable structure.
 *
 * @param data Pointer to the environment variable structure to be freed.
 */
void	free_ev(void *data)
{
	t_env_var	*ev;

	if (data != NULL)
	{
		ev = (t_env_var *)data;
		if (ev->name != NULL)
			free(ev->name);
		if (ev->value != NULL)
			free(ev->value);
		free(ev);
	}
}
