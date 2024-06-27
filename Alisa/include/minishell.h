
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
	t_node_type				type;
	union
	{
		t_cmd		cmd;
		t_error		s_error;
		t_logic		logic;
		t_pipe		pipe;
		t_redir		redir;
		t_brace		brace;
		
	} u_data;
}							t_ast;

typedef enum e_quote_mode
{
	SINGLE,
	DOUBLE,
	OUTSIDE
}							t_quote_mode;

typedef struct s_exp_context
{
	t_quote_mode			quote_mode;
	int						position;
	char					*exp_buffer;
	int						capacity;
	int						buf_pos;
	t_list					**tkn_list;
	bool					is_empty_qts;
	t_list					*wildcard_list;
}							t_exp_context;


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




// ----- add ----- //
void reset_readline(bool redisplay);
t_tkn_type get_pipe(char *input, size_t *len, t_shell *shell);
t_tkn_type get_ampersand(char *input, size_t *len, t_shell *shell);












#endif
