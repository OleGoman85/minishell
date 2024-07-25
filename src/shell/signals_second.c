/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 12:18:25 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 13:02:16 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

void	reset_readline(bool redisplay)
{
	rl_replace_line("", 0);
	rl_on_new_line();
	if (redisplay)
		rl_redisplay();
}

void	conf_signal(int signum, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(signum, &sa, NULL) == -1)
		error_msg_simple("sigaction", "error");
}

void	signal_std(void)
{
	conf_signal(SIGINT, on_sigint_std);
	conf_signal(SIGQUIT, SIG_IGN);
}

void	signals_ignore(void)
{
	conf_signal(SIGINT, SIG_IGN);
	conf_signal(SIGQUIT, SIG_IGN);
}

void	signals_default(void)
{
	conf_signal(SIGINT, SIG_DFL);
	conf_signal(SIGQUIT, SIG_DFL);
}
