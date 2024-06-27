
#include "minishell.h"


char	*display_type(t_tkn_type tkn_type)
{
	if (tkn_type == T_TEXT)
        return ("word");
    if (tkn_type == T_HDOC)
        return ("<<");
	if (tkn_type == T_APPEND)
        return (">>");
    if (tkn_type == T_PIPE)
        return ("|");
	if (tkn_type == T_OR)
        return ("||");
    if (tkn_type == T_BRACE_START)
        return ("(");
    if (tkn_type == T_BRACE_END)
        return (")");
    if (tkn_type == T_AND)
        return ("&&");
    if (tkn_type == T_INPUT)
        return ("<");
    if (tkn_type == T_OUTPUT)
        return (">");
    return ("[unknown token]");
}

char	*get_value(t_list *tkn)
{
	char	*value;
	t_tkn	*tkn_content;

	value = NULL;
	if (tkn != NULL)
	{
		tkn_content = (t_tkn *)tkn->content;
		value = tkn_content->value;
	}
	return (value);
}

t_tkn_type	get_type(t_list *tkn_node)
{
	t_tkn	*tkn_content;

	tkn_content = (t_tkn *)tkn_node->content;
	return (tkn_content->type);
}

bool	in_tkn_list(t_list **tokens, int type_count, ...)
{
	va_list		types;
	t_tkn_type	tkn_type;
	bool		found;

	found = false;
	if (tokens == NULL || *tokens == NULL)
		return (false);
	tkn_type = get_type(*tokens);
	va_start(types, type_count);
	while (type_count-- > 0)
	{
		if (tkn_type == va_arg(types, t_tkn_type))
		{
			found = true;
			break ;
		}
	}
	va_end(types);
	return (found);
}

t_ast	*build_node_logic(t_ast *first_expr, t_tkn_type op, t_ast *second_expr,
		t_shell *shell)
{
	t_ast	*new_node;

	new_node = NULL;
	if (!first_expr || !second_expr)
		return (record_synt_err(display_type(op), shell));
	init_node(&new_node, LOGIC, shell);
	new_node->u_data.logic.logic_op = op;
	new_node->u_data.logic.first = first_expr;
	new_node->u_data.logic.second = second_expr;
	return (new_node);
}

t_ast	*parse_logic(t_list **tkn, t_shell *shell)
{
	t_ast		*first_expr;
	t_ast		*second_expr;
	t_tkn_type	op_type;

	first_expr = parse_pipe(tkn, shell);
	while (in_tkn_list(tkn, 2, T_AND, T_OR))
	{
		op_type = get_type(*tkn);
		*tkn = (*tkn)->next;
		second_expr = parse_pipe(tkn, shell);
		first_expr = build_node_logic(first_expr, op_type, second_expr, shell);
	}
	return (first_expr);
}

t_ast	*parse_pipe(t_list **tkn, t_shell *shell)
{
	t_ast	*input_side;
	t_ast	*output_side;

	input_side = parse_redir(tkn, shell);
	while (*tkn && get_type(*tkn) == T_PIPE)
	{
		*tkn = (*tkn)->next;
		output_side = parse_redir(tkn, shell);
		input_side = build_node_pipe(input_side, output_side, shell);
	}
	return (input_side);
}

int	parse_tokens(t_list *tokens, t_ast **tree, t_shell *shell)
{
	char	*error_token;
	int		error_result;

	*tree = parse_logic(&tokens, shell);
	if (tokens)
	{
		error_token = get_value(tokens);
		record_synt_err(error_token, shell);
	}
	if (shell->syntax_error)
	{
		error_result = display_synt_err(shell);
		return (error_result);
	}
	return (EXIT_SUCCESS);
}
