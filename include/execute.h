/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 06:07:01 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 07:21:52 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"

// ----- EXECUTE ----- //
pid_t	run_pipe_cmd(t_list *pipe_lst, int last_pipe_fd,
			int pipe_fds[2], t_shell *shell);
int		run_cmd(t_ast *ast_node, t_op_status op_status, t_shell *shell);
int		handle_exit_signal(int child_status, bool *printed_newline,
			t_shell *shell);
int		process_cmd(t_cmd *cmd, t_op_status term_status, t_shell *shell);
int		process_logic(t_logic *logic, t_shell *shell);
int		process_brace(t_brace *brace, t_shell *shell);

char	**generate_paths(t_shell *shell);
char	*find_executable_path(char *cmd, t_shell *shell);
char	*locate_executable(char *bin_name, t_shell *sh);

int		execute_command_chain(t_list *pipe_lst, t_shell *shell);
int		collect_child_statuses(pid_t last_exec_pid, int pipe_count,
			t_shell *shell);
int		process_pipeline(t_ast *ast_node, t_shell *shell);
int		process_input_redirection(t_redir *redirect, t_shell *shell);
int		process_output_redirection(t_redir *redirect, t_shell *shell);
int		process_redirection(t_redir *redirect, t_shell *shell);

#endif
