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

/**
 * @brief Handles token substitution and insertion into the context.
 *
 * Processes the given tokens for substitution and manages their 
 * insertion
 * into the provided context based on the presence of leading and 
 * trailing spaces.
 *
 * @param tokens Double pointer to the array of tokens to be processed.
 * @param context Pointer to the substitution context structure.
 * @param shell Pointer to the shell structure.
 * @param str The original string from which the tokens are derived.
 * 
 * @return If a token remains unprocessed, returns the last token. 
 * Otherwise, returns NULL.
 *
 * @details
 * - Checks if the original string starts or ends with a space.
 * - If tokens are provided:
 *   - If the string does not start with a space, the first token is 
 * expanded and inserted.
 *   - If the string starts with a space, it inserts a token directly.
 *   - For each token:
 *     - Inserts the token into the context.
 *     - If the current token is the last and the string does not end 
 * with a space, returns this token.
 *     - Otherwise, adds the token to the tracked list in the context.
 * - If no tokens are provided, inserts a token directly into the context.
 */
char	*handle_tokens(char **tokens, t_subst_context *context, t_shell *shell,
		char *str)
{
	bool	starts_with_space;
	bool	ends_with_space;

	starts_with_space = (str[0] == ' ');
	ends_with_space = (str[ft_strlen(str) - 1] == ' ');
	if (*tokens)
	{
		if (!starts_with_space && (expand_subst_buffer(*tokens++,
					context, shell), !*tokens))
			return (insert_tkn(context, shell));
		else if (starts_with_space)
			insert_tkn(context, shell);
		while (*tokens)
		{
			insert_tkn(context, shell);
			if (!*(tokens + 1) && !ends_with_space)
				return (*tokens);
			lstadd_back_tracked(*tokens++, context->tkn_list,
				COMMAND_TRACK, shell);
		}
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
