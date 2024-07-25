/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 08:35:52 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/24 08:35:52 by ogoman           ###   ########.fr       */
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

/**
 * @brief Inserts the current buffer as a new token into the context 
 * token list.
 *
 * This function checks the current buffer in the context and adds its 
 * contents
 * to the list of tokens. If the buffer is empty and the is_empty_qts 
 * flag is set,
 * an empty string is added. If the buffer contains data, it is copied 
 * into a new token
 * and added to the list. If there is a wildcard list in the context, 
 * the function
 * processes filenames.
 *
 * @param context Pointer to the substitution context containing information 
 * about the buffer and tokens.
 * @param shell Pointer to the shell structure for memory management.
 * @return Always returns NULL.
 */
void	*insert_tkn(t_subst_context *context, t_shell *shell)
{
	char	*new_tkn;

	new_tkn = NULL;
	if (context->buf_pos == 0 && context->is_empty_qts)
	{
		new_tkn = strdup_tracked("", COMMAND_TRACK, shell);
		context->is_empty_qts = false;
	}
	else if (context->buf_pos != 0)
	{
		context->subst_buffer[context->buf_pos] = '\0';
		new_tkn = strdup_tracked(context->subst_buffer, COMMAND_TRACK, shell);
		context->buf_pos = 0;
	}
	else
		return (NULL);
	if (new_tkn != NULL)
		lstadd_back_tracked(new_tkn, context->tkn_list, COMMAND_TRACK, shell);
	if (context->wildcard_list)
		process_filename(context, shell);
	return (NULL);
}

/**
 * @brief Adds the value of a variable to the substitution buffer context.
 *
 * This function increases the capacity of the substitution buffer in the 
 * context
 * to accommodate the new value and appends it to the current buffer. It 
 * also updates
 * the list of wildcard characters.
 *
 * @param var_value The value of the variable to add to the buffer.
 * @param context Pointer to the substitution context containing information 
 * about the buffer and wildcards.
 * @param shell Pointer to the shell structure for memory management.
 */
void	expand_subst_buffer(char *var_value, t_subst_context *context,
		t_shell *shell)
{
	char	*new_buffer;
	size_t	value_len;
	size_t	new_capacity;

	value_len = ft_strlen(var_value);
	new_capacity = context->capacity + value_len;
	locate_wildcards(var_value, context, shell);
	new_buffer = calloc_tracked(new_capacity, sizeof(char), COMMAND_TRACK,
			shell);
	ft_strlcpy(new_buffer, context->subst_buffer, context->buf_pos + 1);
	ft_strlcat(new_buffer, var_value, context->buf_pos + value_len + 1);
	context->subst_buffer = new_buffer;
	context->buf_pos += value_len;
	context->capacity = new_capacity;
}

/**
 * @brief Converts a linked list into an array of strings.
 *
 * This function takes a linked list and creates a new array of strings
 * containing the elements of the list. The last element of the array is set 
 * to NULL.
 *
 * @param list Pointer to the pointer to the first element of the linked list.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the array of strings or NULL if memory allocation fails.
 */
char	**create_string_array(t_list **list, t_shell *shell)
{
	t_list	*node;
	char	**array;
	int		list_size;
	int		index;

	list_size = ft_lstsize(*list);
	array = (char **)calloc_tracked(list_size + 1, sizeof(char *),
			COMMAND_TRACK, shell);
	if (!array)
		return (NULL);
	node = *list;
	index = 0;
	while (node != NULL)
	{
		array[index] = (char *)node->content;
		node = node->next;
		index++;
	}
	array[index] = NULL;
	return (array);
}

// /**
//  * @brief Processes and adds tokens to the substitution context.
//  *
//  * This function parses a string into tokens, processes each token, and 
//  * adds it
//  * to the token list in the substitution context. If a token starts with 
//  * a space,
//  * it is added separately. If a token does not end with a space, it is 
//  * added to the end of the list.
//  *
//  * @param tokens Pointer to the array of tokens.
//  * @param context Pointer to the substitution context.
//  * @param shell Pointer to the shell structure for memory management.
//  * @param str The original string that was split into tokens.
//  * @return Pointer to the last token if it does not end with a space, 
//  *or NULL.
//  */
// char	*handle_tokens(char **tokens, t_subst_context *context, t_shell *shell,
// 		char *str)
// {
// 	bool	starts_with_space;
// 	bool	ends_with_space;

// 	starts_with_space = (str[0] == ' ');
// 	ends_with_space = (str[ft_strlen(str) - 1] == ' ');
// 	if (*tokens)
// 	{
// 		if (!starts_with_space)
// 		{
// 			expand_subst_buffer(*tokens++, context, shell);
// 			if (!*tokens)
// 				return (insert_tkn(context, shell));
// 		}
// 		else
// 			insert_tkn(context, shell);
// 		while (*tokens)
// 		{
// 			insert_tkn(context, shell);
// 			if (!*(tokens + 1) && !ends_with_space)
// 				return (*tokens);
// 			lstadd_back_tracked(*tokens++, context->tkn_list, COMMAND_TRACK,
// 				shell);
// 		}
// 	}
// 	else
// 		insert_tkn(context, shell);
// 	return (NULL);
// }

/**
 * @brief Processes tokens when the input string does not start with a space.
 *
 * This function expands the substitution buffer with the tokens and adds tokens
 * to the list.
 * If the last token does not end with a space, it returns the last token.
 *
 * @param tokens Array of tokens to process.
 * @param context Substitution context containing the buffer and token list.
 * @param shell Pointer to the shell structure for memory management.
 * @param str Original string that was tokenized.
 * @return Pointer to the last token if it does not end with a space, or NULL.
 */
static char	*process_tokens(char **tokens, t_subst_context *context,
		t_shell *shell, char *str)
{
	bool	ends_with_space;

	ends_with_space = (str[ft_strlen(str) - 1] == ' ');
	expand_subst_buffer(*tokens++, context, shell);
	if (!*tokens)
		return (insert_tkn(context, shell));
	while (*tokens)
	{
		insert_tkn(context, shell);
		if (!*(tokens + 1) && !ends_with_space)
			return (*tokens);
		lstadd_back_tracked(*tokens++, context->tkn_list, COMMAND_TRACK,
			shell);
	}
	return (NULL);
}

/**
 * @brief Handles token processing based on the input string and token list.
 *
 * This function checks if the input string starts with a space and calls the
 * appropriate function
 * to process the tokens.
 *
 * @param tokens Array of tokens to process.
 * @param context Substitution context containing the buffer and token list.
 * @param shell Pointer to the shell structure for memory management.
 * @param str Original string that was tokenized.
 * @return Pointer to the last token if it does not end with a space, or NULL.
 */
char	*handle_tokens(char **tokens, t_subst_context *context, t_shell *shell,
		char *str)
{
	bool	starts_with_space;

	starts_with_space = (str[0] == ' ');
	if (*tokens)
	{
		if (!starts_with_space)
			return (process_tokens(tokens, context, shell, str));
		else
			insert_tkn(context, shell);
	}
	else
		insert_tkn(context, shell);
	return (NULL);
}

/**
 * @brief Splits a string into tokens and processes them.
 *
 * This function splits the input string into tokens using spaces as 
 * delimiters and
 * calls `handle_tokens` to process each token. If the string is empty 
 * or contains
 * only one token, it is returned as is.
 *
 * @param context Pointer to the substitution context.
 * @param str The input string to be split into tokens.
 * @param shell Pointer to the shell structure for memory management.
 * @return Pointer to the last token if it does not end with a space, 
 * or NULL.
 */
char	*tokenize_words(t_subst_context *context, char *str, t_shell *shell)
{
	char	**tokens;
	char	*result;

	if (strlen(str) == 0)
		return (str);
	tokens = split_tracked(str, ' ', COMMAND_TRACK, shell);
	if (!tokens || !*tokens)
		return (insert_tkn(context, shell));
	if (ft_strcmp(str, tokens[0]) == 0)
		return (str);
	result = handle_tokens(tokens, context, shell, str);
	return (result);
}
