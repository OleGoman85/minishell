
#include "minishell.h"

void	init_node(t_ast **ast_node, t_node_type node_type, t_shell *shell)
{
	t_ast	*new_node;

	new_node = calloc_tracked(1, sizeof(t_ast), COMMAND_TRACK, shell);
	new_node->type = node_type;
	*ast_node = new_node;
}

t_ast	*build_node_cmd(char **argv, t_shell *shell)
{
	t_ast	*node;

	init_node(&node, CMD, shell);
	node->u_data.cmd.cmd_args = argv;
	return (node);
}

t_ast	*build_node_brace(t_ast *child, t_shell *shell)
{
	t_ast	*node;

	if (!child)
		return (record_synt_err("(", shell));
	init_node(&node, BRACE, shell);
	node->u_data.brace.command = child;
	return (node);
}

t_ast	*build_node_redir(t_tkn_type direction, t_list *tk_filename,
		t_ast *child, t_shell *shell)
{
	t_ast	*node;

	if (!tk_filename || get_type(tk_filename) != T_TEXT)
		return (record_synt_err(display_type(direction), shell));
	init_node(&node, REDIR, shell);
	node->u_data.redir.redir_type = direction;
	node->u_data.redir.filename = get_value(tk_filename);
	node->u_data.redir.command = child;
	return (node);
}

t_ast	*build_node_pipe(t_ast *left, t_ast *right, t_shell *shell)
{
	t_ast	*node;

	if (!left || !right)
		return (record_synt_err("|", shell));
	init_node(&node, PIPE, shell);
	node->u_data.pipe.input_side = left;
	node->u_data.pipe.output_side = right;
	return (node);
}
