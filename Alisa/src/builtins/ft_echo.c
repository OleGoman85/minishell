#include "minishell.h"

static bool	has_flag(char **cmd_args, int *index, char flag) // to builtin utils
{
	bool	flag_found;
	int		char_pos;

	flag_found = false;
	while (cmd_args[*index] && cmd_args[*index][0] == '-')
	{
		char_pos = 1;
		while (cmd_args[*index][char_pos] == flag)
			char_pos++;
		if (cmd_args[*index][char_pos] == '\0' && char_pos > 1)
		{
			flag_found = true;
			(*index)++;
		}
		else
			return (flag_found);
	}
	return (flag_found);
}

int	ft_echo(t_cmd *cmd, t_shell *shell)
{
	bool	add_newline;
	int		first_arg;
	int		i;

	add_newline = true;
	i = 1;
	first_arg = 1;
	if (has_flag(cmd->cmd_args, &i, 'n'))
		add_newline = false;
	if (cmd->cmd_args[i])
	{
		write_and_track(cmd->cmd_args[i], STDOUT_FILENO, shell);
		i++;
		first_arg = 0;
	}
	while (cmd->cmd_args[i])
	{
		if (!first_arg)
			write_and_track(" ", STDOUT_FILENO, shell);
		write_and_track(cmd->cmd_args[i], STDOUT_FILENO, shell);
		i++;
	}
	if (add_newline)
		write_and_track("\n", STDOUT_FILENO, shell);
	return (EXIT_SUCCESS);
}
