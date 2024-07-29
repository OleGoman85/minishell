/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:46:16 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:25:09 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Runs the main shell loop, handling user input and executing commands.
 *
 * This function continuously prompts the user for input, processes the input,
 * and executes the corresponding commands. It also handles cleanup and 
 * signal interruptions.
 *
 * @param shell Pointer to the shell structure.
 * @return The exit status of the last command executed.
 */
int	run_shell(t_shell *shell)
{
	char	*input_str;

	while (true)
	{
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

/**
 * @brief Cleans up shell resources, such as temporary files and memory.
 *
 * This function removes temporary files, clears tracked memory, and resets 
 * the syntax error state.
 *
 * @param shell Pointer to the shell structure.
 */
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

/**
 * @brief Gets user input from the standard input or heredoc.
 *
 * This function prompts the user for input depending on the input type 
 * (standard input or heredoc)
 * and sets the appropriate signal handlers.
 *
 * @param input_type The type of input to be read (standard input or heredoc).
 * @return The input string read from the user.
 */
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
