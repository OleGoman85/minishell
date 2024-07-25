/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:03:03 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 13:58:32 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reads directory entries and adds them to the file list.
 *
 * This function opens the specified directory and reads its entries. For each 
 * entry, except for the current (".") 
 * and parent ("..") directories, it creates a full file name and adds it to 
 * the provided file list. If the root path 
 * is not ".", the full file name includes the root path; otherwise, only the 
 * file name is added.
 *
 * @param directory Pointer to the opened directory.
 * @param root Path to the root directory.
 * @param file_list Pointer to the file list where found files will be added.
 * @param shell Pointer to the shell structure for memory management.
 */
static void	read_directory_files(DIR *directory, char *root, t_list **file_list,
		t_shell *shell)
{
	struct dirent	*file_record;
	char			*file_name;

	file_record = read_directory(directory, shell);
	while (file_record != NULL)
	{
		if (ft_strcmp(file_record->d_name, ".")
			&& ft_strcmp(file_record->d_name, ".."))
		{
			if (ft_strcmp(root, "."))
				file_name = strjoin_tracked(root, file_record->d_name,
						COMMAND_TRACK, shell);
			else
				file_name = strdup_tracked(file_record->d_name, COMMAND_TRACK,
						shell);
			add_file_to_list(file_name, file_list, shell);
		}
		file_record = read_directory(directory, shell);
	}
}

/**
 * @brief Retrieves a list of files from the specified directory.
 *
 * This function extracts the root path from the substitution buffer context, 
 * opens the corresponding directory,
 * reads its contents using the `read_directory_files` function, and returns
 *  the list of found files. If the directory 
 * cannot be opened or is empty, the function returns NULL.
 *
 * @param context Pointer to the substitution context for retrieving the root
 *  path.
 * @param shell Pointer to the shell structure for memory management.
 * 
 * @return Pointer to the list of files, or NULL if the directory is empty 
 * or cannot be opened.
 */
t_list	*retrive_files(t_subst_context *context, t_shell *shell)
{
	DIR		*directory;
	char	*root;
	char	*slash_pos;
	t_list	*file_list;
	int		len;

	file_list = NULL;
	slash_pos = ft_strrchr(context->subst_buffer, '/');
	if (slash_pos == NULL)
		root = strdup_tracked(".", COMMAND_TRACK, shell);
	else
	{
		len = slash_pos - context->subst_buffer + 1;
		root = calloc_tracked(len + 1, sizeof(char), COMMAND_TRACK, shell);
		ft_strlcpy(root, context->subst_buffer, len + 1);
	}
	directory = open_directory(root, shell);
	if (directory != NULL)
	{
		read_directory_files(directory, root, &file_list, shell);
		close_directory(directory, shell);
	}
	return (file_list);
}

/**
 * @brief Inserts a new node at the beginning of the list.
 *
 * This function inserts a new node at the start of the list.
 *
 * @param file_list Pointer to the pointer to the first node of the list.
 * @param new_node Pointer to the new node to be inserted.
 */
static void	insert_at_beginning(t_list **file_list, t_list *new_node)
{
	if (*file_list != NULL)
		(*file_list)->prev = new_node;
	new_node->next = *file_list;
	*file_list = new_node;
}

/**
 * @brief Inserts a new node in the middle of the list after the 
 * specified node.
 *
 * This function inserts a new node after a given node in the list.
 *
 * @param cur_node Pointer to the node after which the new node will be 
 * inserted.
 * @param new_node Pointer to the new node to be inserted.
 */
static void	insert_in_middle(t_list *cur_node, t_list *new_node)
{
	if (cur_node->next != NULL)
		cur_node->next->prev = new_node;
	new_node->next = cur_node->next;
	cur_node->next = new_node;
	new_node->prev = cur_node;
}

/**
 * @brief Adds a new file to the linked list in alphabetical order.
 *
 * This function creates a new list node containing the file name and 
 * inserts it into the linked list in the
 * correct position to maintain alphabetical order. If the list is empty,
 *  the new node becomes the first element.
 * If the file name is lexicographically less than the first node's name,
 *  the new node is inserted at the beginning.
 * Otherwise, the function finds the appropriate position in the list and
 *  inserts the new node.
 *
 * @param filename The name of the file to be added to the list.
 * @param file_list Pointer to the pointer to the first node of the list.
 * @param shell Pointer to the shell structure used for memory management.
 */
void	add_file_to_list(char *filename, t_list **file_list, t_shell *shell)
{
	t_list	*new_file;
	t_list	*cur_node;

	new_file = ft_lstnew(strdup_tracked(filename, COMMAND_TRACK, shell));
	alloc_check(new_file, filename, shell);
	track_memory(new_file, COMMAND_TRACK, shell);
	if (*file_list == NULL || ft_strcmp(filename, (*file_list)->content) < 0)
	{
		insert_at_beginning(file_list, new_file);
		return ;
	}
	cur_node = *file_list;
	while (cur_node->next != NULL)
	{
		if (ft_strcmp(cur_node->next->content, filename) >= 0)
			break ;
		cur_node = cur_node->next;
	}
	insert_in_middle(cur_node, new_file);
}
