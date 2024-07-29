/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 09:49:18 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:03:28 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Checks if the entire string consists of digits.
 *
 * @param str The input string.
 * @return true if the string contains only digits, false otherwise.
 */
static bool	ft_isdigit_str(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

/**
 * Converts a string to an unsigned long long integer considering the sign.
 *
 * @param str The input string.
 * @param sign The sign of the number.
 * @param overflow Pointer to a boolean that will be set to true if overflow
 * occurs.
 * @return The converted unsigned long long integer.
 */
static unsigned long long	custom_atoull(const char *str, int sign,
		bool *overflow)
{
	unsigned long long	result;
	int					digit;

	result = 0;
	*overflow = false;
	while (*str)
	{
		digit = *str - '0';
		if (result > (ULLONG_MAX - digit) / 10)
		{
			*overflow = true;
			return (ULLONG_MAX);
		}
		result = result * 10 + digit;
		str++;
	}
	if ((sign == -1 && result > (unsigned long long)LLONG_MAX + 1)
		|| (sign == 1 && result > (unsigned long long)LLONG_MAX))
	{
		*overflow = true;
		return (ULLONG_MAX);
	}
	return (result);
}

static int	check_numeric_argument(const char *arg, long long *num,
			t_shell *shell, int *error_status)
{
	const char	*str;
	int			sign;
	bool		overflow;
	char		*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, " ");
	str = skip_whitespace_and_sign(trimmed_arg, &sign);
	if (!trimmed_arg || *str == '\0' || !ft_isdigit_str(str))
	{
		free(trimmed_arg);
		*error_status = 2;
		return (handle_exit_errors(2, (char *)arg, shell));
	}
	*num = custom_atoull(str, sign, &overflow);
	free(trimmed_arg);
	if (overflow || (*num == LLONG_MIN && sign != -1) || (*num == LLONG_MAX
			&& sign != 1))
	{
		*error_status = 2;
		return (handle_exit_errors(2, (char *)arg, shell));
	}
	*num *= sign;
	return (-1);
}

/**
 * Parses the exit arguments and determines the appropriate exit status.
 *
 * @param cmd The command structure containing arguments.
 * @param num Pointer to store the converted number.
 * @param shell The shell context.
 * @param error_status Pointer to store the error status.
 * @return The normalized exit status.
 */
static int	parse_exit_arguments(t_cmd *cmd, long long *num,
			t_shell *shell, int *error_status)
{
	int	status;

	*num = 0;
	*error_status = 0;
	status = check_numeric_argument(cmd->cmd_args[1], num, shell, error_status);
	if (status != -1)
		return (status);
	if (cmd->cmd_args[2])
	{
		*error_status = 1;
		return (handle_exit_errors(1, NULL, shell), 1);
	}
	return ((unsigned char)*num);
}

/**
 * The main exit function for the shell, handling the exit
 * command logic.
 *
 * @param cmd The command structure containing arguments.
 * @param shell The shell context.
 * @return The exit status if there are too many arguments,
 * otherwise exits the shell.
 */
int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	long long	num;
	int			exit_status;
	int			error_status;

	if (!cmd->cmd_args[1])
		clean_exit(0, shell);
	if (ft_strcmp(cmd->cmd_args[1], "--") == 0)
	{
		if (cmd->cmd_args[2])
			return (handle_exit_errors(1, NULL, shell), 1);
		return (0);
	}
	exit_status = parse_exit_arguments(cmd, &num, shell, &error_status);
	if (error_status == 1)
		return (exit_status);
	if (error_status == 2)
		clean_exit(2, shell);
	clean_exit(exit_status, shell);
	exit(exit_status);
}
