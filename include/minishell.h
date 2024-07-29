/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:46:35 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:19:59 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <signal.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <limits.h>
# include <linux/limits.h>
# include <curses.h>
# include <term.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "structs.h"
# include "execute.h"
# include "builtins.h"
# include "parse.h"
# include "utils.h"
# include "substitution.h"

# define DEFAULT_PATH "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
	:/usr/local/sbin:/opt/bin:/opt/sbin"

void		rl_replace_line(const char *text, int clear_undo);
void		rl_clear_history(void);

extern sig_atomic_t	g_signal;

//________MAIN________//
t_tkn_type	get_tkn_type(char *input, size_t *len, t_shell *shell);
t_tkn_type	get_braces(char *input, size_t *len);
t_tkn_type	get_ampersand(char *input, size_t *len, t_shell *shell);
t_tkn_type	get_pipe(char *input, size_t *len, t_shell *shell);
t_tkn_type	get_redirect(char *input, char c, size_t *len, t_shell *shell);
t_tkn_type	get_word(char *input, size_t *len, t_shell *shell);
ssize_t		write_and_track(const char *str, int fd, t_shell *shell);
t_list		*create_ev_list(char **env_vars, t_shell *shell);
t_list		*get_ev(char *target, t_list *ev_list);
t_tkn		*create_token(t_tkn_type tkn_type, char *tkn_value,
				size_t tkn_len, t_shell *shell);
void		setup_shell(t_shell *shell, char **env_vars);
void		update_shell_level(t_shell *shell);
void		clean_exit(int exit_code, t_shell *shell);
void		add_ev(char *name, char *value, t_list **ev_list, t_shell *shell);
void		change_ev_val(t_list *ev_ptr, char *new_val,
				bool retain_old, t_shell *shell);
void		free_ev(void *data);
void		exit_on_error(const char *cause, char *msg,
				int exit_code, t_shell *shell);
void		exit_on_sys_error(const char *cause, int errnum, t_shell *shell);
void		cleanup_shell(t_shell *shell);
void		*record_synt_err(char *tkn, t_shell *shell);
void		reset_readline(bool redisplay);
void		conf_signal(int signum, void (*handler)(int));
void		signal_std(void);
void		signals_ignore(void);
void		signals_default(void);
void		signal_heredoc(void);
void		on_sigint_std(int signum);
void		lstadd_front_tracked(void *content, t_list **lst,
				t_mem_trackers tracker, t_shell *shell);
void		lstadd_back_tracked(void *content, t_list **lst,
				t_mem_trackers tracker, t_shell *shell);
void		alloc_check(t_list *node, void *mem_ptr, t_shell *shell);
void		del_node(t_list **target, t_list **lst_head,
				void (*free_data)(void *), bool should_free);
void		on_sigint_doc(int signum);
void		*manage_memory(void *mem_ptr, t_mem_trackers tracker,
				t_shell *shell);
void		*calloc_tracked(size_t num_elements, size_t el_size,
				t_mem_trackers tracker, t_shell *shell);
void		track_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell);
void		error_msg_simple(char *prefix, char *msg);
void		handle_quotes_and_escapes(char *input, size_t *len,
				bool *in_quote, char *c);
char		*get_ev_name(t_list *ev_node);
char		*strjoin_tracked(char const *s1, char const *s2,
				t_mem_trackers tracker, t_shell *shell);
char		**split_tracked(char const *str, char c, t_mem_trackers tracker,
				t_shell *shell);
char		*strdup_tracked(char const *str, t_mem_trackers tracker,
				t_shell *shell);
char		*get_ev_value(t_list *ev_node);
char		*get_input(t_input_type input_type);
char		**create_ev_array(t_shell *shell);
int			main(int argc, char **argv, char **envp);
int			display_ev_list(bool export_mode, t_list *ev, t_shell *shell);
int			error_msg_errno(char *cause, t_shell *shell);
int			error_msg(char *cause, char *faulty_el, char *msg, t_shell *shell);
int			is_special_tkn(int c);
int			display_synt_err(t_shell *shell);
int			tokenize_input(char *input_str, t_list **tokens, t_shell *shell);
int			process_input(char *input_str, t_shell *shell);
int			run_shell(t_shell *shell);

#endif
