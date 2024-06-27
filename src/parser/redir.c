
#include "minishell.h"


t_ast	*assemble_cmd(t_ast *prefix, t_ast *suffix, t_ast *cmd)
{
	 t_ast *last_redir;

    if (prefix)
    {
        last_redir = get_last_redirection_list(prefix);
        if (suffix)
        {
            last_redir->u_data.redir.command = suffix;
            last_redir = get_last_redirection_list(suffix);
            last_redir->u_data.redir.command = cmd;
        }
        else
        {
            last_redir->u_data.redir.command = cmd;
        }
        return prefix;
    }
    else if (suffix)
    {
        last_redir = get_last_redirection_list(suffix);
        last_redir->u_data.redir.command = cmd;
        return suffix;
    }
    return cmd;
}

t_ast	*get_last_redirection_list(t_ast *node)
{
	while (node && node->type == REDIR
		&& node->u_data.redir.command
		&& node->u_data.redir.command->type == REDIR)
		node = node->u_data.redir.command;
	return (node);
}


t_ast	*parse_redir(t_list **token, t_shell *sh)
{
	t_ast	*prefix;
	t_ast	*suffix;
	t_ast	*command;

	prefix = parse_redirection_list(token, NULL, sh);
	if (sh->syntax_error)
		return (NULL);
	command = parse_brace(token, sh);
	suffix = parse_redirection_list(token, command, sh);
	if (sh->syntax_error)
		return (NULL);
	return (assemble_cmd(prefix, suffix, command));
}

bool	parse_new_command_arg(t_list **token, t_ast *command, t_shell *sh)
{
	if (in_tkn_list(token, 1, T_TEXT) && command)
	{
		add_arg_to_array(&command->u_data.cmd.cmd_args, get_value(*token), sh);
		*token = (*token)->next;
		return (true);
	}
	return (false);
}

bool	is_in_sequential_redirection_list(t_list **token, t_ast *command)
{
	if (in_tkn_list(token, 4, T_INPUT, T_OUTPUT, T_APPEND,
			T_HDOC))
		return (true);
	if (in_tkn_list(token, 1, T_TEXT) && command)
		return (true);
	return (false);
}


t_ast	*parse_redirection_list(t_list **token, t_ast *command, t_shell *sh)
{
	t_ast	*first;
	t_ast	*last;
	t_ast	*new;

	first = NULL;
	while (is_in_sequential_redirection_list(token, command))
	{
		if (parse_new_command_arg(token, command, sh))
			continue ;
		new = build_node_redir(get_type(*token), (*token)->next, NULL, sh);
		if (!new)
			return (NULL);
		if (!first)
		{
			first = new;
			last = new;
		}
		else
		{
			last->u_data.redir.command = new;
			last = new;
		}
		*token = (*token)->next->next;
	}
	return (first);
}
