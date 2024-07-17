
#ifndef MINISHELL_H
# define MINISHELL_H


# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <limits.h>


# include "libft.h"

# include <curses.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>

void	rl_replace_line(const char *text, int clear_undo);
void	rl_clear_history(void);

extern sig_atomic_t			g_signal;

// ----- SHELL ----- //
typedef struct s_shell
{
	t_list					*ev_list;
	char					*home_dir;
	t_list					*mem_tracker[3];
	t_list					*temp_files;
	char					*syntax_error;
	bool					is_main;
	int						prev_cmd_status;
}							t_shell;

typedef enum e_input_type
{
	IN_STD,
	IN_HDOC
}							t_input_type;


typedef enum e_tkn_type
{
	T_PIPE,
	T_AND,
	T_OR,
	T_TEXT,
	T_APPEND,
	T_INPUT,
	T_OUTPUT,
	T_HDOC,
	T_BRACE_START,
	T_BRACE_END,
	T_ERR
}							t_tkn_type;

typedef struct s_tkn
{
	t_tkn_type				type;
	char					*value;
}							t_tkn;


typedef enum e_node_type
{
	CMD,
	BRACE,
	LOGIC,
	PIPE,
	REDIR,
	ERROR
}							t_node_type;

typedef struct s_cmd
{
	char					**cmd_args;
}							t_cmd;

typedef struct s_pipe
{
	struct s_ast			*input_side;
	struct s_ast			*output_side;
}							t_pipe;

typedef struct s_logic
{
	struct s_ast			*first;
	struct s_ast			*second;
	t_tkn_type			logic_op;
}							t_logic;

typedef struct s_redir
{
	t_tkn_type			redir_type;
	struct s_ast			*command;
	char					*filename;
}							t_redir;

typedef struct s_brace
{
	struct s_ast			*command;
}							t_brace;

typedef struct s_error
{
	char					*invalid_tkn;
}							t_error;

typedef struct s_ast
{
	t_node_type				node_type;
	union
	{
		t_cmd		cmd;
		t_error		error;
		t_logic		logic;
		t_pipe		pipe;
		t_redir		redir;
		t_brace		brace;
		
	} node_content;
}							t_ast;

typedef enum e_quote_mode
{
	SINGLE,
	DOUBLE,
	OUTSIDE
}							t_quote_mode;

typedef struct s_subst_context
{
	int						pos;
	char					*subst_buffer;
	int						buf_pos;
	int						capacity;
	t_quote_mode			quote_mode;
	bool					is_empty_qts;
	t_list					**tkn_list;
	t_list					*wildcard_list;
}							t_subst_context;


typedef enum e_op_status
{
	OP_COMPLETE,
	OP_TERMINATE
}							t_op_status;

# define PIPE_INPUT 0
# define PIPE_OUTPUT 1

// ----- BUILTINS ----- //
typedef int					(*t_bltn_func)(t_cmd *, t_shell *);

typedef struct s_bltn_cmd
{
	char					*cmd_name;
	t_bltn_func			cmd_func;
}							t_bltn_cmd;

#define NUM_BUILTINS 7

// ----- ENV VARIABLES ----- //

typedef struct s_env_var
{
	char					*name;
	char					*value;
}							t_env_var;


typedef enum e_mem_trackers
{
	UNTRACKED,
	CORE_TRACK,
	COMMAND_TRACK
}							t_mem_trackers;

# define DEFAULT_PATH "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin\
:/usr/local/sbin:/opt/bin:/opt/sbin"

//________MAIN________//
void setup_shell(t_shell *shell, char **env_vars);
void update_shell_level(t_shell *shell);
void clean_exit(int exit_code, t_shell *shell);
int main(int argc, char **argv, char **envp);

int display_ev_list(bool export_mode, t_list *ev, t_shell *shell);
char **create_ev_array(t_shell *shell);

t_list *create_ev_list(char **env_vars, t_shell *shell);
void add_ev(char *name, char *value, t_list **ev_list, t_shell *shell);
void change_ev_val(t_list *ev_ptr, char *new_val, bool retain_old, t_shell *shell);
t_list *get_ev(char *target, t_list *ev_list);
char *get_ev_name(t_list *ev_node);
char *get_ev_value(t_list *ev_node);
void free_ev(void *data);

void exit_on_error(const char *cause, char *msg, int exit_code, t_shell *shell);
void exit_on_sys_error(const char *cause, int errnum, t_shell *shell);
int error_msg_errno(char *cause, t_shell *shell);
int error_msg(char *cause, char *faulty_el, char *msg, t_shell *shell);
void error_msg_simple(char *prefix, char *msg);

int is_space(int c);
int is_special_tkn(int c);
void *record_synt_err(char *tkn, t_shell *shell);
int display_synt_err(t_shell *shell);
t_tkn_type get_tkn_type(char *input, size_t *len, t_shell *shell);
t_tkn_type get_braces(char *input, size_t *len);
t_tkn_type get_ampersand(char *input, size_t *len, t_shell *shell);
t_tkn_type get_pipe(char *input, size_t *len, t_shell *shell);
t_tkn_type get_redirect(char *input, char c, size_t *len, t_shell *shell);
t_tkn_type get_word(char *input, size_t *len, t_shell *shell);
int tokenize_input(char *input_str, t_list **tokens, t_shell *shell);
t_tkn *create_token(t_tkn_type tkn_type, char *tkn_value, size_t tkn_len, t_shell *shell);
int process_input(char *input_str, t_shell *shell);

int run_shell(t_shell *shell);
void cleanup_shell(t_shell *shell);
char *get_input(t_input_type input_type);
void reset_readline(bool redisplay);
void conf_signal(int signum, void (*handler)(int));
void signal_std(void);
void signals_ignore(void);
void signals_default(void);
void signal_heredoc(void);
void on_sigint_std(int signum);
void on_sigint_doc(int signum);

void *manage_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell);
void *calloc_tracked(size_t num_elements, size_t el_size, t_mem_trackers tracker, t_shell *shell);
void track_memory(void *mem_ptr, t_mem_trackers tracker, t_shell *shell);
char *strjoin_tracked(char const *s1, char const *s2, t_mem_trackers tracker, t_shell *shell);
char **split_tracked(char const *str, char c, t_mem_trackers tracker, t_shell *shell);
char *strdup_tracked(char const *str, t_mem_trackers tracker, t_shell *shell);
ssize_t write_and_track(const char *str, int fd, t_shell *shell);
void lstadd_front_tracked(void *content, t_list **lst, t_mem_trackers tracker, t_shell *shell);
void lstadd_back_tracked(void *content, t_list **lst, t_mem_trackers tracker, t_shell *shell);
void alloc_check(t_list *node, void *mem_ptr, t_shell *shell);
void del_node(t_list **target, t_list **lst_head, void (*free_data)(void *), bool should_free);

//________BUILTINS________//
void init_builtins(t_bltn_cmd *commands);
t_bltn_func fetch_builtin_cmd(char *command);

int ft_cd(t_cmd *cmd, t_shell *shell);
int ft_echo(t_cmd *cmd, t_shell *shell);
int ft_env(t_cmd *cmd, t_shell *shell);
int ft_export(t_cmd *cmd, t_shell *shell);
int	ft_pwd(t_cmd *cmd, t_shell *shell);
int ft_unset(t_cmd *cmd, t_shell *shell);
int ft_exit(t_cmd *cmd, t_shell *sh);


//________EXECUTE________//
int run_cmd(t_ast *ast_node, t_op_status op_status, t_shell *shell);
int handle_exit_signal(int child_status, bool *printed_newline, t_shell *shell);
int process_cmd(t_cmd *cmd, t_op_status term_status, t_shell *shell);
int process_logic(t_logic *logic, t_shell *shell);
int process_brace(t_brace *brace, t_shell *shell);

char **generate_paths(t_shell *shell);
char *find_executable_path(char *cmd, t_shell *shell);
char *locate_executable(char *bin_name, t_shell *sh);

int execute_command_chain(t_list *pipe_lst, t_shell *shell);
int collect_child_statuses(pid_t last_exec_pid, int pipe_count, t_shell *shell);
int process_pipeline(t_ast *ast_node, t_shell *shell);
pid_t run_pipe_cmd(t_list *pipe_lst, int last_pipe_fd, int pipe_fds[2], t_shell *shell);

int process_input_redirection(t_redir *redirect, t_shell *shell);
int process_output_redirection(t_redir *redirect, t_shell *shell);
int process_redirection(t_redir *redirect, t_shell *shell);

//________PARSE________//
t_ast *build_node_logic(t_ast *first_expr, t_tkn_type op, t_ast *second_expr, t_shell *shell);
t_ast *build_node_cmd(char **cmd_args, t_shell *shell);
t_ast *build_node_pipe(t_ast *input_node, t_ast *output_node, t_shell *shell);
t_ast *build_node_redir(t_tkn_type redir_type, t_list *token, t_ast *child_node, t_shell *shell);
t_ast *build_node_brace(t_ast *inner_node, t_shell *shell);

char *get_tkn_label(t_tkn_type tkn_type);
char *get_value(t_list *tkn);
t_tkn_type get_type(t_list *tkn_node);
t_ast *parse_cmd(t_list **tokens, t_shell *shell);
t_ast *parse_brace(t_list **tokens, t_shell *shell);
int parse_tokens(t_list *tokens, t_ast **syntax_tree, t_shell *shell);

t_ast *link_redir_to_cmd(t_ast *redir_sequence, t_ast *cmd);
t_ast *assemble_cmd(t_ast *redir_prefix, t_ast *redir_suffix, t_ast *cmd);
t_ast *find_redir_tail(t_ast *redir_head);
t_ast *process_redir(t_list **tokens, t_shell *shell);
bool add_cmd_arg(t_list **tokens, t_ast *cmd_node, t_shell *shell);
bool is_valid_redir(t_list **tokens, t_ast *cmd_node);
t_ast *append_redir_node(t_ast *redir_chain, t_ast *new_redir);
t_ast *extract_redirections(t_list **tokens, t_ast *cmd_node, t_shell *shell);

//________SUBSTITUTION________//
bool process_asterisk(const char **file_ptr, const char **mask_ptr, int *pos, t_subst_context *context);
bool wildcard_check(const char *file, const char *wildcard_mask, int pos, t_subst_context *context);
bool in_wildcard_list(int pos, t_subst_context *context);
void locate_wildcards(char *str, t_subst_context *context, t_shell *shell);
bool is_file_visible(const char *filepath, t_subst_context *context);
void *insert_tkn(t_subst_context *context, t_shell *shell);
void expand_subst_buffer(char *var_value, t_subst_context *context, t_shell *shell);
char **create_string_array(t_list **list, t_shell *shell);
char *handle_tokens(char **tokens, t_subst_context *context, t_shell *shell, char *str);
char *tokenize_words(t_subst_context *context, char *str, t_shell *shell);

t_ast *resolve_ast_content(t_ast *node, t_shell *shell);
void resolve_ev(char *input, t_subst_context *context, t_shell *shell);
char *extract_ev_value(char *input, t_subst_context *context, t_shell *shell);
void process_quotes(char quote_char, char *arg, t_subst_context *context, t_shell *shell);
void process_unquoted_chars(char *arg, t_subst_context *context, t_shell *shell);
void resolve_arg(char *arg, t_list **arg_list, t_shell *shell);
void process_filename(t_subst_context *ctx, t_shell *shell);
t_list *retrive_files(t_subst_context *context, t_shell *shell);

void add_file_to_list(char *filename, t_list **file_list, t_shell *shell);
t_list *clean_file_list(t_list *file_list, t_subst_context *context);

//________UTILS________//
void append_str(char ***args, char *str, t_shell *shell);
pid_t wait_for_child(int *status, t_shell *shell);
int create_pipe(int pipe_fds[2], t_shell *shell);
int execute_program(const char *path, char *const args[], char *const env[], t_shell *shell);
pid_t create_process(t_shell *shell);
DIR *open_directory(const char *path, t_shell *shell);
int close_directory(DIR *directory, t_shell *shell);
struct dirent *read_directory(DIR *directory, t_shell *shell);
int open_file(const char *path, int mode_flags, mode_t permissions, t_shell *shell);
int close_file(int fd, t_shell *shell);
int get_file_status(const char *restrict file_path, struct stat *restrict file_stat, t_shell *shell);
int duplicate_fd(int source_fd, int target_fd, t_shell *shell);











#endif
