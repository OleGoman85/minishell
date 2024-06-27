
#include "minishell.h"

t_ast	*parse_brace(t_list **token, t_shell *sh)
{
	t_ast	*content;

	if (in_tkn_list(token, 1, T_BRACE_START))
	{
		*token = (*token)->next;
		content = parse_logic(token, sh);
		if (*token && get_type(*token) == T_BRACE_END)
		{
			*token = (*token)->next;
			if ((*token) && in_tkn_list(token, 1, T_TEXT))
				return (record_synt_err(get_value(*token), sh));
			return (build_node_brace(content, sh));
		}
		else if (*token == NULL)
			return (record_synt_err("\\n", sh));
		else
			return (record_synt_err(get_value(*token), sh));
	}
	else if (in_tkn_list(token, 1, T_BRACE_END))
		return (record_synt_err(")", sh));
	return (parse_cmd(token, sh));
}

t_ast	*parse_cmd(t_list **token, t_shell *sh)
{
	t_list	*current;
	int		argc;
	char	**argv;
	int		i;

	current = *token;
	argc = 0;
	while (in_tkn_list(&current, 1, T_TEXT))
	{
		argc++;
		current = current->next;
	}
	if (argc == 0)
		return (NULL);
	argv = calloc_tracked((argc + 1), sizeof(char *), COMMAND_TRACK, sh);
	i = 0;
	while (i < argc)
	{
		argv[i++] = get_value(*token);
		*token = (*token)->next;
	}
	argv[argc] = NULL;
	return (build_node_cmd(argv, sh));
}
