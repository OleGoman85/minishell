/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitution.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:23:06 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 07:23:54 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SUBSTITUTION_H
# define SUBSTITUTION_H

# include "minishell.h"

//________SUBSTITUTION________//
t_list	*clean_file_list(t_list *file_list, t_subst_context *context);
t_list	*retrive_files(t_subst_context *context, t_shell *shell);
t_ast	*resolve_ast_content(t_ast *node, t_shell *shell);
bool	process_asterisk(const char **file_ptr, const char **mask_ptr,
			int *pos, t_subst_context *context);
bool	wildcard_check(const char *file, const char *wildcard_mask,
			int pos, t_subst_context *context);
bool	in_wildcard_list(int pos, t_subst_context *context);
bool	is_file_visible(const char *filepath, t_subst_context *context);
void	locate_wildcards(char *str, t_subst_context *context,
			t_shell *shell);
void	*insert_tkn(t_subst_context *context, t_shell *shell);
void	resolve_ev(char *input, t_subst_context *context, t_shell *shell);
void	expand_subst_buffer(char *var_value, t_subst_context *context,
			t_shell *shell);
void	process_quotes(char quote_char, char *arg,
			t_subst_context *context, t_shell *shell);
void	add_file_to_list(char *filename, t_list **file_list,
			t_shell *shell);
void	process_unquoted_chars(char *arg,
			t_subst_context *context, t_shell *shell);
void	resolve_arg(char *arg, t_list **arg_list, t_shell *shell);
void	process_filename(t_subst_context *ctx, t_shell *shell);
char	**create_string_array(t_list **list, t_shell *shell);
char	*handle_tokens(char **tokens, t_subst_context *context,
			t_shell *shell, char *str);
char	*tokenize_words(t_subst_context *context, char *str,
			t_shell *shell);
char	*extract_ev_value(char *input, t_subst_context *context,
			t_shell *shell);

#endif