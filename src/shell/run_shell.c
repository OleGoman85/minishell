
#include "minishell.h"

int	run_shell(t_shell *shell)
{
	char	*input_str;

	while (true)
	{
		//g_signal = 0;//uncomment!!
		//printf("g_signal run_shell: %d\n", g_signal);	
		input_str = get_input(IN_STD);
		if (g_signal == SIGINT)
			shell->prev_cmd_status = 130;
		if (!input_str)
			clean_exit(shell->prev_cmd_status, shell);
		track_memory(input_str, COMMAND_TRACK, shell);
		if (ft_strlen(input_str) > 0)
		{
			add_history(input_str);
			shell->prev_cmd_status = process_input(input_str, shell);
		}
		else
			reset_readline(true);
		cleanup_shell(shell);
	}
	return (shell->prev_cmd_status);
}

void	cleanup_shell(t_shell *shell)
{
	while (shell->is_main && shell->temp_files)
	{
		if (unlink(shell->temp_files->content) == -1)
			error_msg_errno("unlink", shell);
		shell->temp_files = shell->temp_files->next;
	}
	ft_lstclear(&(shell->mem_tracker[COMMAND_TRACK]), free);
	shell->syntax_error = NULL;
}

char	*get_input(t_input_type input_type)
{
	char	*input_str;

	g_signal = 0;
	input_str = NULL;
	reset_readline(false);
	if (input_type == IN_STD)
	{
		signal_std();
		input_str = readline("minishell$ ");
		signals_ignore();
	}
	else if (input_type == IN_HDOC)
	{
		signal_heredoc();
		input_str = readline("heredoc> ");
		signals_ignore();
	}
	return (input_str);
}
