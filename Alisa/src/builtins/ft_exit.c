
#include "minishell.h"

int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	(void)cmd;
	(void)shell;
	clean_exit(shell->prev_cmd_status, shell);
	return (EXIT_SUCCESS);
}