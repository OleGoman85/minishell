/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:32:54 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:07:24 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints an error message and exits the program.
 *
 * This function prints an error message in the format 
 * `minishell: <msg>: <cause>`,
 * then calls `clean_exit` to perform any necessary cleanup and exit the program
 * with the specified exit code.
 *
 * @param cause The cause of the error.
 * @param msg The specific error message.
 * @param exit_code The exit code to use when exiting the program.
 * @param shell Pointer to the shell structure for cleanup.
 */
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

/**
 * @brief Prints a system error message and exits the program.
 *
 * This function prints an error message in the format 
 * `minishell: <strerror(errnum)>: <cause>`,
 * where `<strerror(errnum)>` is the system error message 
 * corresponding to the error number `errnum`.
 * It then calls `clean_exit` to perform any necessary cleanup and 
 * exit the program with `EXIT_FAILURE`.
 *
 * @param cause The cause of the error.
 * @param errnum The system error number.
 * @param shell Pointer to the shell structure for cleanup.
 */
void	exit_on_sys_error(const char *cause, int errnum, t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(strerror(errnum), STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(cause, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	clean_exit(EXIT_FAILURE, shell);
}

/**
 * @brief Prints an error message with a system error description.
 *
 * This function prints an error message in the format
 * `minishell: <cause>: <system error>`.
 * The system error message is obtained using `perror` which uses 
 * `errno` to get the system error
 * string.
 *
 * @param cause The cause of the error.
 * @param shell Pointer to the shell structure (though unused in 
 * this function).
 * @return The exit code `EXIT_FAILURE`.
 */
int	error_msg_errno(char *cause, t_shell *shell)
{
	write_and_track("minishell: ", STDERR_FILENO, shell);
	perror(cause);
	return (EXIT_FAILURE);
}
/**
 * @brief Prints a detailed error message with additional context.
 *
 * This function prints an error message in the format
 * `minishell: <cause><faulty_el><msg>`,
 * where `<faulty_el>` and `<msg>` are optional and are 
 * included if provided.
 *
 * @param cause The cause of the error.
 * @param faulty_el Additional context about the faulty 
 * element (can be NULL).
 * @param msg Additional message describing the error.
 * @param shell Pointer to the shell structure for tracking and printing.
 * @return The exit code `EXIT_FAILURE`.
 */

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

/**
 * @brief Prints a simple error message with a system error description.
 *
 * This function prints an error message in the 
 * format `<prefix>: <msg>: <system error>`,
 * where `<system error>` is the description of the system error 
 * obtained using `strerror(errno)`.
 *
 * @param prefix Prefix for the error message.
 * @param msg The specific error message.
 */
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
