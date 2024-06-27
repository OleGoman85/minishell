/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 13:02:25 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 15:46:31 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	del(void *content)
{
	if (content != NULL)
	{
		free(content);
	}
}

void	mem_clear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next;

	if (lst == NULL || del == NULL)
		return ;
	current = *lst;
	while (current != NULL)
	{
		next = current->next;
		del(current->content);
		free(current);
		current = next;
	}
	*lst = NULL;
}

t_list	*create_node(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
	{
		del(new_node);
		return (NULL);
	}
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

void	add_node_to_end(t_list **list, t_list *new_node)
{
	t_list	*current;

	if (*list == NULL)
	{
		*list = new_node;
	}
	else
	{
		current = *list;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	void	*new_content;
	t_list	*new_node;

	new_list = NULL;
	while (lst != NULL)
	{
		new_content = f(lst->content);
		if (new_content == NULL)
		{
			mem_clear(&new_list, del);
			return (NULL);
		}
		new_node = create_node(new_content);
		if (new_node == NULL)
		{
			del(new_content);
			mem_clear(&new_list, del);
			return (NULL);
		}
		add_node_to_end(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
