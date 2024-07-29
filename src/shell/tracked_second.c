/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracked_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:05:44 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:26:25 by aarbenin         ###   ########.fr       */
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
