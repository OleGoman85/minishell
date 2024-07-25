/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracked.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:53:16 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 08:02:16 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Manages memory allocation and tracking.
 *
 * This function checks if a memory allocation was successful. If the allocation
 * fails, it triggers an error. If the allocation is successful and tracking is
 * required, it adds the allocated memory to the appropriate tracker.
 *
 * @param mem_ptr Pointer to the allocated memory.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 * @return The memory pointer if allocation was successful, or terminates
 * the program if not.
 */
void	*manage_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell)
{
	if (!mem_ptr)
		exit_on_error("Memory allocation", strerror(errno),
			EXIT_FAILURE, shell);
	if (tracker != UNTRACKED)
		track_memory(mem_ptr, tracker, shell);
	return (mem_ptr);
}

/**
 * @brief Allocates memory for an array of elements and tracks it.
 *
 * This function uses `ft_calloc` to allocate memory for an array of elements and
 * initializes the allocated memory to zero. It then tracks the memory
 * if a tracker
 * is specified.
 *
 * @param num_elements Number of elements to allocate.
 * @param el_size Size of each element in bytes.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 * @return Pointer to the allocated memory.
 */

void	*calloc_tracked(size_t num_elements, size_t el_size,
			t_mem_trackers tracker, t_shell *shell)
{
	return (manage_memory(ft_calloc(num_elements, el_size), tracker, shell));
}

/**
 * @brief Tracks a given memory pointer in the shell's memory tracker.
 *
 * This function adds a memory pointer to the shell's memory tracker list. If
 * memory allocation for the tracker list fails, it frees the memory and exits
 * the program with an error.
 *
 * @param mem_ptr Pointer to the memory to be tracked.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 */
void	track_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell)
{
	t_list	*mem_entry;

	mem_entry = ft_lstnew(mem_ptr);
	alloc_check(mem_entry, mem_ptr, shell);
	if (!mem_entry)
	{
		free(mem_ptr);
		exit_on_error("Memory allocation", strerror(errno),
			EXIT_FAILURE, shell);
	}
	ft_lstadd_front(&shell->mem_tracker[tracker], mem_entry);
}

/**
 * @brief Joins two strings and tracks the result.
 *
 * This function concatenates two strings and tracks the resulting memory. If
 * the string joining operation fails, it will trigger an error.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 * @return Pointer to the concatenated string.
 */
char	*strjoin_tracked(char const *s1, char const *s2, t_mem_trackers tracker,
	t_shell *shell)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	return (manage_memory(joined, tracker, shell));
}
/**
 * @brief Splits a string into an array of substrings and tracks each element.
 *
 * This function splits a string based on a delimiter character and tracks each
 * substring in the resulting array. If the splitting operation fails, it will
 * trigger an error.
 *
 * @param str The string to be split.
 * @param c The delimiter character.
 * @param tracker Enum indicating the type of memory tracker to use.
 * @param shell Pointer to the shell structure for tracking memory.
 * @return Array of substrings.
 */

char	**split_tracked(char const *str, char c, t_mem_trackers tracker,
			t_shell *shell)
{
	char	**split_arr;
	int		i;

	i = 0;
	split_arr = ft_split(str, c);
	manage_memory(split_arr, tracker, shell);
	while (split_arr && split_arr[i])
		manage_memory(split_arr[i++], tracker, shell);
	return (split_arr);
}

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
