/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:47:48 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:26:07 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_heredoc(void)
{
	conf_signal(SIGINT, on_sigint_doc);
	conf_signal(SIGQUIT, SIG_IGN);
}

void	on_sigint_std(int signum)
{
	g_signal = signum;
	write(1, "\n", 1);
	reset_readline(true);
}

void	on_sigint_doc(int signum)
{
	struct termios	orig_settings;
	struct termios	mod_settings;

	g_signal = signum;
	if (tcgetattr(STDIN_FILENO, &orig_settings) == -1)
	{
		error_msg_simple("tcgetattr", "error");
		return ;
	}
	mod_settings = orig_settings;
	mod_settings.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &mod_settings) == -1)
	{
		error_msg_simple("tcsetattr", "error");
		return ;
	}
	if (ioctl(STDIN_FILENO, TIOCSTI, "\n") == -1)
		error_msg_simple("ioctl", "error");
	if (tcsetattr(STDIN_FILENO, TCSANOW, &orig_settings) == -1)
		error_msg_simple("tcsetattr", "restore error");
	reset_readline(true);
	rl_done = 1;
}
