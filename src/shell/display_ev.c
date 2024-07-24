
#include "minishell.h"

int	display_ev_list(bool export_mode, t_list *ev, t_shell *shell)
{
	while (ev != NULL)
	{
		if (!export_mode && !get_ev_value(ev))
		{
			ev = ev->next;
			continue ;
		}
		if (export_mode)
			write_and_track("declare -x ", STDOUT_FILENO, shell);
		write_and_track(get_ev_name(ev), STDOUT_FILENO, shell);
		if (get_ev_value(ev))
			write_and_track("=", STDOUT_FILENO, shell);
		if (export_mode && get_ev_value(ev))
		{
			write_and_track("\"", STDOUT_FILENO, shell);
			write_and_track(get_ev_value(ev), STDOUT_FILENO, shell);
			write_and_track("\"", STDOUT_FILENO, shell);
		}
		else if (get_ev_value(ev))
			write_and_track(get_ev_value(ev), STDOUT_FILENO, shell);
		write_and_track("\n", STDOUT_FILENO, shell);
		ev = ev->next;
	}
	return (EXIT_SUCCESS);
}

static void	fill_ev_array(char **ev_array, t_list *ev_list, t_shell *shell)
{
	int		i;
	char	*ev_value;
	char	*ev_name;

	i = 0;
	while (ev_list)
	{
		ev_value = get_ev_value(ev_list);
		if (ev_value)
		{
			ev_name = strjoin_tracked(get_ev_name(ev_list), "=", COMMAND_TRACK,
					shell);
			ev_array[i] = strjoin_tracked(ev_name, ev_value, COMMAND_TRACK,
					shell);
			i++;
		}
		ev_list = ev_list->next;
	}
	ev_array[i] = NULL;
}

char	**create_ev_array(t_shell *shell)
{
	int		ev_count;
	char	**env_array;

	ev_count = ft_lstsize(shell->ev_list);
	env_array = calloc_tracked(ev_count + 1, sizeof(char *), COMMAND_TRACK,
			shell);
	if (!env_array)
		return (NULL);
	fill_ev_array(env_array, shell->ev_list, shell);
	return (env_array);
}
