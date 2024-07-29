/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_therd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 13:53:54 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 13:58:03 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Processes an argument string, performing variable substitution, 
 * quote handling, and special character processing.
 *
 * This function initializes a context for tracking the substitution state, 
 * processes each character of the input string
 * based on the current quote mode, performs environment variable substitution,
 *  tilde expansion, and handles special characters.
 * Processed tokens are added to the argument list.
 *
 * @param arg The argument string to process.
 * @param arg_list Pointer to the list where processed arguments will be added.
 * @param shell Pointer to the shell structure containing runtime environment 
 * information.
 */

void	resolve_arg(char *arg, t_list **arg_list, t_shell *shell)
{
	t_subst_context	context;

	context.pos = 0;
	context.subst_buffer = calloc_tracked(ft_strlen(arg) + 1, sizeof(char),
			COMMAND_TRACK, shell);
	context.buf_pos = 0;
	context.capacity = ft_strlen(arg) + 1;
	context.quote_mode = OUTSIDE;
	context.is_empty_qts = false;
	context.tkn_list = arg_list;
	context.wildcard_list = NULL;
	while (arg[context.pos])
	{
		if (context.quote_mode == SINGLE)
			process_quotes('\'', arg, &context, shell);
		else if (context.quote_mode == DOUBLE)
			process_quotes('\"', arg, &context, shell);
		else if (context.quote_mode == OUTSIDE)
			process_unquoted_chars(arg, &context, shell);
		if (arg[context.pos])
			context.pos++;
	}
	if (context.quote_mode != OUTSIDE)
		exit_on_error("parsing", "missing closing quote", EXIT_FAILURE, shell);
	insert_tkn(&context, shell);
}

/**
 * @brief Processes file names and adds them to the token list.
 *
 * This function completes the current string in the buffer, retrieves a list 
 * of files matching the path,
 * filters the files based on the given pattern, and adds the filtered files 
 * to the token list.
 * It then resets the buffer after processing.
 *
 * @param ctx Pointer to the substitution context.
 * @param shell Pointer to the shell structure for memory management.
 */
void	process_filename(t_subst_context *ctx, t_shell *shell)
{
	t_list	*file_lst;

	ctx->subst_buffer[ctx->buf_pos] = '\0';
	file_lst = clean_file_list(retrive_files(ctx, shell), ctx);
	while (file_lst)
	{
		lstadd_back_tracked(file_lst->content, ctx->tkn_list, COMMAND_TRACK,
			shell);
		file_lst = file_lst->next;
	}
	ctx->buf_pos = 0;
	ctx->subst_buffer[ctx->buf_pos] = '\0';
}

/**
 * @brief Cleans up the list of files by removing entries that do not match 
 * the pattern or are hidden.
 *
 * This function filters the provided list of files based on pattern matching 
 * from the substitution buffer (`context->subst_buffer`)
 * and visibility checks. Files that do not match the pattern or are hidden 
 * are removed from the list.
 *
 * @param file_list The list of files to be filtered.
 * @param context The substitution context containing the pattern buffer.
 * @return The cleaned list of files, containing only those that match the 
 * pattern and are not hidden.
 */
t_list	*clean_file_list(t_list *file_list, t_subst_context *context)
{
	t_list	*cur_file;
	t_list	*previous;
	t_list	*next;
	char	*filename;

	cur_file = file_list;
	previous = NULL;
	while (cur_file)
	{
		filename = (char *)cur_file->content;
		next = cur_file->next;
		if (wildcard_check(filename, context->subst_buffer, 0, context)
			&& is_file_visible(filename, context))
			previous = cur_file;
		else
		{
			if (previous)
				previous->next = next;
			else
				file_list = next;
			del_node(&cur_file, NULL, NULL, false);
		}
		cur_file = next;
	}
	return (file_list);
}
