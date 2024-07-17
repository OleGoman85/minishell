
#include "minishell.h"



int	ft_exit(t_cmd *cmd, t_shell *sh)
{
	(void)cmd;
	(void)sh;
	clean_exit(sh->prev_cmd_status, sh);
	return (EXIT_SUCCESS);
}