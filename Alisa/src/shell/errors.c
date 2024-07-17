#include "minishell.h"

void	exit_on_error(const char *cause, char *msg, int exit_code,
		t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cause, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	clean_exit(exit_code, shell);
}

void	exit_on_sys_error(const char *cause, int errnum, t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cause, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	clean_exit(EXIT_FAILURE, shell);
}

int	error_msg_errno(char *cause, t_shell *shell)
{
	write_and_track("minishell: ", STDERR_FILENO, shell);
	perror(cause);
	return (EXIT_FAILURE);
}

int	error_msg(char *cause, char *faulty_el, char *msg, t_shell *shell)
{
	write_and_track("minishell: ", STDERR_FILENO, shell);
	write_and_track(cause, STDERR_FILENO, shell);
	if (faulty_el)
		write_and_track(faulty_el, STDERR_FILENO, shell);
	write_and_track(msg, STDERR_FILENO, shell);
	write_and_track("\n", STDERR_FILENO, shell);
	return (EXIT_FAILURE);
}

void	error_msg_simple(char *prefix, char *msg)
{
	char	*error_message;

	error_message = strerror(errno);
	ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(error_message, STDERR_FILENO);
}
