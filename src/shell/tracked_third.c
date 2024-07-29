/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracked_therd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:08:13 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:26:34 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Deletes a specific node from a list and optionally frees its data.
 *
 * This function removes a target node from a list and updates the list's head
 * pointer. If specified, it also frees the node's data. 
 *
 * @param target Pointer to the node to delete.
 * @param lst_head Pointer to the head of the list.
 * @param free_data Function to free the node's data.
 * @param should_free Boolean indicating if the node's data should be freed.
 */
void	del_node(t_list **target, t_list **lst_head,
		void (*free_data)(void *), bool should_free)
{
	t_list	*current;

	if (!target || !*target || !lst_head)
		return ;
	current = *target;
	if (current->prev)
		current->prev->next = current->next;
	else
		*lst_head = current->next;
	if (current->next)
		current->next->prev = current->prev;
	if (should_free)
		ft_lstdelone(current, free_data);
}
