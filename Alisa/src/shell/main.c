#include "minishell.h"

sig_atomic_t	g_signal = 0;

void	setup_shell(t_shell *shell, char **env_vars)
{
	errno = 0;
	shell->is_main = true;
	shell->prev_cmd_status = EXIT_SUCCESS;
	shell->mem_tracker[UNTRACKED] = NULL;
	shell->mem_tracker[CORE_TRACK] = NULL;
	shell->mem_tracker[COMMAND_TRACK] = NULL;
	shell->temp_files = NULL;
	shell->ev_list = create_ev_list(env_vars, shell);
	update_shell_level(shell);
	shell->syntax_error = NULL;
}

void	update_shell_level(t_shell *shell)
{
	t_list	*shlvl_ev;
	int		current_lvl;
	char	*lvl_str;

	shlvl_ev = get_ev("SHLVL", shell->ev_list);
	if (!shlvl_ev)
	{
		add_ev("SHLVL", "1", &(shell->ev_list), shell);
        return ;
	}
	current_lvl = ft_atoi(get_ev_value(shlvl_ev)) + 1;
	lvl_str = manage_memory(ft_itoa(current_lvl), COMMAND_TRACK, shell);
	if (current_lvl < 0)
		change_ev_val(shlvl_ev, "0", false, shell);
	else if (current_lvl > 1000)
	{
		error_msg("shell level (", lvl_str,") too high, resetting to 1", shell);
		change_ev_val(shlvl_ev, "1", false, shell);
	}
	else
		change_ev_val(shlvl_ev, lvl_str, false, shell);
}
void	clean_exit(int exit_code, t_shell *shell)
{
	if (shell)
	{
		if (shell->is_main && isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDERR_FILENO);
		cleanup_shell(shell);
		ft_lstclear(&shell->ev_list, free_ev);
		ft_lstclear(&shell->mem_tracker[CORE_TRACK], free);
	}
	rl_clear_history();
	exit(exit_code);
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		last_command_status;

	(void) argv;
	if (argc != 1)
		exit(EXIT_FAILURE);
	setup_shell(&shell, envp);
	signals_ignore();
	last_command_status = run_shell(&shell);
	clean_exit(last_command_status, &shell);
}
