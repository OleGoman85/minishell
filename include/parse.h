/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:21:38 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 07:22:14 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include"minishell.h"

//________PARSE________//
t_tkn_type	get_type(t_list *tkn_node);
t_ast		*parse_cmd(t_list **tokens, t_shell *shell);
t_ast		*parse_brace(t_list **tokens, t_shell *shell);
t_ast		*build_node_logic(t_ast *first_expr, t_tkn_type op,
				t_ast *second_expr, t_shell *shell);
t_ast		*build_node_cmd(char **cmd_args, t_shell *shell);
t_ast		*build_node_pipe(t_ast *input_node, t_ast *output_node,
				t_shell *shell);
t_ast		*build_node_redir(t_tkn_type redir_type, t_list *token,
				t_ast *child_node, t_shell *shell);
t_ast		*build_node_brace(t_ast *inner_node, t_shell *shell);
t_ast		*parse_pipe(t_list **tkn_list, t_shell *shell);
t_ast		*parse_logic(t_list **tkn_list, t_shell *shell);
t_ast		*link_redir_to_cmd(t_ast *redir_sequence, t_ast *cmd);
t_ast		*assemble_cmd(t_ast *redir_prefix, t_ast *redir_suffix, t_ast *cmd);
t_ast		*find_redir_tail(t_ast *redir_head);
t_ast		*process_redir(t_list **tokens, t_shell *shell);
t_ast		*append_redir_node(t_ast *redir_chain, t_ast *new_redir);
t_ast		*extract_redirections(t_list **tokens, t_ast *cmd_node,
				t_shell *shell);
char		*get_tkn_label(t_tkn_type tkn_type);
char		*get_value(t_list *tkn);
bool		add_cmd_arg(t_list **tokens, t_ast *cmd_node, t_shell *shell);
bool		is_valid_redir(t_list **tokens, t_ast *cmd_node);
int			parse_tokens(t_list *tokens, t_ast **syntax_tree, t_shell *shell);

#endif