
#include "minishell.h"

int	ms_pwd(t_cmd *cmd, t_shell *shell)
{
	char	*ev_pwd;
	char	*current_dir;

	(void)cmd;
	ev_pwd = get_ev_value(get_ev("PWD", shell->ev_list));
	if (ev_pwd)
		write_ext(ev_pwd, STDOUT_FILENO, shell);
	else
	{
		current_dir = getcwd(NULL, 0);
		if (!current_dir)
		{
			exit_on_sys_error("getcwd", errno, shell);
			return (1);
		}
		// manage_memory(current_dir, COMMAND_TRACK, shell);
		write_ext(current_dir, STDOUT_FILENO, shell);
		free(current_dir);
	}
	write_ext("\n", STDOUT_FILENO, shell);
	return (0);
}
