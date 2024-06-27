#include "minishell.h"


t_list	*create_ev_list(char **env_vars, t_shell *shell)
{
	char	*equals_ptr;
	char	*name;
	char	*extracted_name;
	t_list	*ev_list;
	char	*home_val;

	ev_list = NULL;
	while (*env_vars)
	{
		equals_ptr = ft_strchr(*env_vars, '=');
		extracted_name = ft_substr(*env_vars, 0, equals_ptr - *env_vars);
		name = manage_memory(extracted_name, COMMAND_TRACK, shell);
		add_ev(name, equals_ptr + 1, &(ev_list), shell);
		env_vars++;
	}
	if (!get_ev("PATH", ev_list))
		add_ev("PATH", DEFAULT_PATH, &(ev_list), shell);
	home_val = get_ev_value(get_ev("HOME", ev_list));
	if (home_val)
		shell->home_dir = strdup_tracked(home_val, CORE_TRACK, shell);
	else
		shell->home_dir = NULL;
	return (ev_list);
}

void	add_ev(char *name, char *value, t_list **ev_list, t_shell *shell)
{
	t_env_var	*ev;

	ev = calloc_tracked(1, sizeof(t_env_var), UNTRACKED, shell);
	ev->name = strdup_tracked(name, UNTRACKED, shell);
	ev->value = NULL;
	if (value)
		ev->value = strdup_tracked(value, UNTRACKED, shell);
	lstadd_back_tracked(ev, ev_list, UNTRACKED, shell);
}

void	change_ev_val(t_list *ev_ptr, char *new_val,
						bool retain_old, t_shell *shell)
{
	t_env_var	*ev;
	char	*old_val;

	ev = (t_env_var *)(ev_ptr->content);
	old_val = ev->value;
	if (!new_val)
		ev->value = NULL;
	else if (retain_old)
		ev->value = strjoin_tracked(ev->value, new_val, UNTRACKED, shell);
	else
		ev->value = strdup_tracked(new_val, UNTRACKED, shell);
	free(old_val);
}


t_list	*get_ev(char *target, t_list *ev_list)
{
	t_env_var	*current_ev;

	while (ev_list)
	{
		current_ev = (t_env_var *)(ev_list->content);
		if (ft_strcmp(target, current_ev->name) == 0)
			return (ev_list);
		ev_list = ev_list->next;
	}
	return (NULL);
}

char	*get_ev_name(t_list *ev_node)
{
	if (!ev_node)
		return (NULL);
	return (((t_env_var *)(ev_node->content))->name);
}

char	*get_ev_value(t_list *ev_node)
{
	if (!ev_node)
		return (NULL);
	return (((t_env_var *)(ev_node->content))->value);
}
