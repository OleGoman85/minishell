/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:32:29 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 13:37:54 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes the '*' (wildcard) character in the pattern.
 *
 * This function advances the pattern pointer and checks all possible 
 * substrings
 * of the file string for a match with the remaining part of the pattern. 
 * If the 
 * '*' is the last character in the pattern, the function returns true. 
 * Otherwise, 
 * it recursively calls `wildcard_check` for each possible substring.
 *
 * @param file_ptr Pointer to the current position in the file string.
 * @param mask_ptr Pointer to the current position in the pattern string.
 * @param pos Pointer to the current position in the pattern.
 * @param context Pointer to the substitution context structure containing 
 * additional
 *                information for wildcard processing.
 * @return true if a match is found, otherwise false.
 */
bool	process_asterisk(const char **file_ptr, const char **mask_ptr,
		int *pos, t_subst_context *context)
{
	(*mask_ptr)++;
	(*pos)++;
	if (!**mask_ptr)
		return (true);
	while (**file_ptr)
	{
		if (wildcard_check(*file_ptr, *mask_ptr, *pos, context))
			return (true);
		(*file_ptr)++;
	}
	return (false);
}

/**
 * @brief Checks if a string matches a filename considering wildcard masks.
 *
 * This function checks whether a string matches the filename given a mask that
 *  may
 * contain wildcard characters ('*'). The mask can include one or more wildcard
 *  characters
 * that match zero or more characters in the filename.
 *
 * The function iterates through the file and mask strings, comparing 
 * characters one by one,
 * and processes wildcard '*' when encountered.
 *
 * @param file Pointer to the filename string to check.
 * @param wildcard_mask Pointer to the mask string containing wildcards.
 * @param pos Current position in the mask string.
 * @param context Substitution context containing the list of wildcard 
 * positions.
 * @return true if the filename string matches the mask, otherwise false.
 */
bool	wildcard_check(const char *file, const char *wildcard_mask, int pos,
		t_subst_context *context)
{
	while (*file && *wildcard_mask)
	{
		if (*wildcard_mask == '*' && in_wildcard_list(pos, context))
		{
			if (process_asterisk(&file, &wildcard_mask, &pos, context))
				continue ;
			else
				return (false);
		}
		else if (*wildcard_mask++ != *file++)
			return (false);
		pos++;
	}
	while (*wildcard_mask == '*' && in_wildcard_list(pos, context))
	{
		wildcard_mask++;
		pos++;
	}
	if (*wildcard_mask == '\0' && *file == '\0')
		return (true);
	else
		return (false);
}

/**
 * @brief Checks if the current position is in the list of active wildcards.
 *
 * This function iterates through the list of wildcard positions in 
 * the substitution context
 * and checks if the current position is among them.
 *
 * @param pos The current position to check.
 * @param context Pointer to the substitution context structure containing 
 * the wildcard list.
 * @return true if the current position is an active wildcard, otherwise false.
 */
bool	in_wildcard_list(int pos, t_subst_context *context)
{
	t_list	*node;
	int		*current_pos;
	bool	is_match;

	node = context->wildcard_list;
	while (node != NULL)
	{
		current_pos = (int *)(node->content);
		is_match = (*current_pos == pos);
		if (is_match)
			return (true);
		node = node->next;
	}
	return (false);
}

/**
 * @brief Finds the positions of '*' characters in a string and saves them 
 * in the substitution context.
 *
 * This function scans through the string, locates all '*' characters, and 
 * saves their positions
 * relative to the start of the string in the wildcard list in the 
 * substitution context.
 * Positions are saved only if the current quote mode is OUTSIDE.
 *
 * @param str The string to search for '*' characters.
 * @param context Pointer to the substitution context containing information 
 * about the current quote mode and buffer.
 * @param shell Pointer to the shell structure for memory management.
 */
void	locate_wildcards(char *str, t_subst_context *context, t_shell *shell)
{
	int	*wildcard_ptr;
	int	i;

	if (context->quote_mode == OUTSIDE)
	{
		wildcard_ptr = NULL;
		i = 0;
		while (str[i] != '\0')
		{
			if (str[i] == '*')
			{
				wildcard_ptr = calloc_tracked(1, sizeof(int), COMMAND_TRACK,
						shell);
				*wildcard_ptr = context->buf_pos + i;
				lstadd_back_tracked(wildcard_ptr, &context->wildcard_list,
					COMMAND_TRACK, shell);
			}
			i++;
		}
	}
}

/**
 * @brief Checks if a file is visible based on its name and mask.
 *
 * This function checks whether a file is hidden (starts with '.') and if the 
 * mask allows hidden files.
 * If the file is hidden and the mask does not allow hidden files, the 
 * function returns true.
 *
 * @param filepath Path to the file to check.
 * @param context Pointer to the substitution context structure containing 
 * the mask.
 * @return true if the file is hidden and the mask does not allow hidden 
 * files; false otherwise.
 */
bool	is_file_visible(const char *filepath, t_subst_context *context)
{
	const char	*mask_segment;
	const char	*name_segment;
	bool		file_is_hidden;
	bool		mask_allows_hidden;

	mask_segment = ft_strrchr(context->subst_buffer, '/');
	name_segment = ft_strrchr(filepath, '/');
	if (!mask_segment)
		mask_segment++;
	else
		mask_segment = context->subst_buffer;
	if (name_segment)
		name_segment++;
	else
		name_segment = filepath;
	file_is_hidden = (name_segment[0] == '.');
	mask_allows_hidden = (mask_segment[0] != '.');
	if (file_is_hidden && mask_allows_hidden)
		return (true);
	return (false);
}
