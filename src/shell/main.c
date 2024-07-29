/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:37:49 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:07:54 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

sig_atomic_t	g_signal = 0;

/**
 * @brief Initializes the shell environment.
 *
 * This function sets up the shell's internal state, including memory tracking,
 * environment variables, and shell level. It is called at the start of 
 * the shell.
 *
 * @param shell Pointer to the shell structure to initialize.
 * @param env_vars Array of environment variables passed from the system.
 */
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

/**
 * @brief Updates the shell level (SHLVL) environment variable.
 *
 * This function increments the shell level, resets it if it exceeds a maximum 
 * value,
 * or initializes it if it is not already set. The shell level is used to track 
 * the
 * depth of nested shells.
 *
 * @param shell Pointer to the shell structure containing the environment 
 * variables.
 */
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
		error_msg("shell level (", lvl_str, ") too high, resetting to 1",
			shell);
		change_ev_val(shlvl_ev, "1", false, shell);
	}
	else
		change_ev_val(shlvl_ev, lvl_str, false, shell);
}

/**
 * @brief Cleans up and exits the shell program.
 *
 * This function performs necessary cleanup operations such as closing file 
 * descriptors,
 * clearing memory, and printing an exit message. It then terminates the 
 * program with
 * the specified exit code.
 *
 * @param exit_code The exit code to use when terminating the program.
 * @param shell Pointer to the shell structure to clean up.
 */
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

/**
 * @brief The main entry point for the minishell program.
 *
 * This function initializes the shell, sets up signal handling, runs the 
 * shell's main
 * loop, and performs cleanup before exiting.
 *
 * @param argc The number of command-line arguments (should be 1 for 
 * minishell).
 * @param argv Array of command-line arguments (not used).
 * @param envp Array of environment variables passed from the system.
 * @return The exit status of the program.
 */
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
