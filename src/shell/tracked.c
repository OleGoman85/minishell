/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracked.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:53:16 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:26:52 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates a string and tracks the result.
 *
 * This function creates a duplicate of a given string and tracks the memory
 * used for the duplication. If the duplication operation fails, it will trigger
 * an error.
 *
 * @param str The string to duplicate.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 * @return Pointer to the duplicated string.
 */
char	*strdup_tracked(char const *str, t_mem_trackers tracker, t_shell *shell)
{
	char	*dup;

	dup = ft_strdup(str);
	return (manage_memory(dup, tracker, shell));
}

/**
 * @brief Writes a string to a file descriptor and handles any errors.
 *
 * This function writes a string to a specified file descriptor and tracks the
 * number of bytes written. If the write operation fails, it triggers an error.
 *
 * @param str The string to write.
 * @param fd The file descriptor to write to.
 * @param shell Pointer to the shell structure for error handling.
 * @return The number of bytes written, or triggers an error if the write fails.
 */
ssize_t	write_and_track(const char *str, int fd, t_shell *shell)
{
	ssize_t	result;

	if (!str)
		exit_on_sys_error("write", EINVAL, shell);
	result = write(fd, str, strlen(str));
	if (result == -1)
		exit_on_sys_error("write", errno, shell);
	return (result);
}
/**
 * @brief Adds a new node with content to the front of a list and tracks it.
 *
 * This function creates a new list node with the given content, adds it to the
 * front of the specified list, and tracks the memory used for the node. If
 * memory allocation fails, it will trigger an error.
 *
 * @param content Pointer to the content of the new node.
 * @param lst Pointer to the list to which the node will be added.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 */

void	lstadd_front_tracked(void *content, t_list **lst,
			t_mem_trackers tracker, t_shell *shell)
{
	t_list	*node;

	node = ft_lstnew(content);
	alloc_check(node, content, shell);
	ft_lstadd_front(lst, node);
	track_memory(node, tracker, shell);
}

/**
 * @brief Adds a new node with content to the back of a list and tracks it.
 *
 * This function creates a new list node with the given content, adds it to the
 * back of the specified list, and tracks the memory used for the node. If
 * memory allocation fails, it will trigger an error.
 *
 * @param content Pointer to the content of the new node.
 * @param lst Pointer to the list to which the node will be added.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 */
void	lstadd_back_tracked(void *content, t_list **lst, t_mem_trackers tracker,
		t_shell *shell)
{
	t_list	*node;

	node = ft_lstnew(content);
	alloc_check(node, content, shell);
	ft_lstadd_back(lst, node);
	track_memory(node, tracker, shell);
}

/**
 * @brief Checks if a list node was successfully allocated and handles errors.
 *
 * This function verifies that a list node was allocated successfully. If the
 * allocation failed, it frees the associated memory and triggers an error.
 *
 * @param node Pointer to the list node.
 * @param mem_ptr Pointer to the associated memory.
 * @param shell Pointer to the shell structure for error handling.
 */
void	alloc_check(t_list *node, void *mem_ptr, t_shell *shell)
{
	if (!node)
	{
		free(mem_ptr);
		exit_on_error("Memory allocation", strerror(errno),
			EXIT_FAILURE, shell);
	}
}
