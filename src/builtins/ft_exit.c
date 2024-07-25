#include "minishell.h"

/**
 * Skips leading whitespace and determines the sign of the number.
 *
 * @param str The input string.
 * @param sign Pointer to an integer where the sign will be stored.
 * @return The pointer to the first non-whitespace character in the string.
 */
static const char	*skip_whitespace_and_sign(const char *str, int *sign)
{
	*sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

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
 * @param overflow Pointer to a boolean that will be set to true if overflow occurs.
 * @return The converted unsigned long long integer.
 */
static unsigned long long	custom_atoull(const char *str, int sign, bool *overflow)
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
	if ((sign == -1 && result > (unsigned long long)LLONG_MAX + 1) ||
		(sign == 1 && result > (unsigned long long)LLONG_MAX))
	{
		*overflow = true;
		return (ULLONG_MAX);
	}
	return (result);
}

/**
 * Handles exit errors by displaying an appropriate error message.
 *
 * @param error_code The error code to handle.
 * @param arg The argument causing the error.
 * @param shell The shell context.
 * @return The error status.
 */
static int	handle_exit_errors(int error_code, char *arg, t_shell *shell)
{
	if (error_code == 1)
		return (error_msg("exit: ", NULL, "too many arguments", shell));
	else if (error_code == 2)
		return (error_msg("exit: ", arg, ": numeric argument required", shell));
	return (EXIT_FAILURE);
}

/**
 * Checks if the provided argument is a valid numeric argument.
 *
 * @param arg The argument to check.
 * @param num Pointer to store the converted number.
 * @param shell The shell context.
 * @param error_status Pointer to store the error status.
 * @return 2 if the argument is invalid, -1 otherwise.
 */
static int	check_numeric_argument(const char *arg, long long *num,
			t_shell *shell, int *error_status)
{
	int		sign;
	const char	*str;
	bool		overflow;
	char		*trimmed_arg;

	trimmed_arg = ft_strtrim(arg, " ");
	if (!trimmed_arg || *(str = skip_whitespace_and_sign(trimmed_arg, &sign)) == '\0' || !ft_isdigit_str(str))
	{
		free(trimmed_arg);
		*error_status = 2;
		return (handle_exit_errors(2, (char *)arg, shell));
	}
	*num = custom_atoull(str, sign, &overflow);
	free(trimmed_arg);
	if (overflow || (*num == LLONG_MIN && sign != -1) || (*num == LLONG_MAX && sign != 1))
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
 * The main exit function for the shell, handling the exit command logic.
 *
 * @param cmd The command structure containing arguments.
 * @param shell The shell context.
 * @return The exit status if there are too many arguments, otherwise exits the shell.
 */
int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	long long	num;
	int		exit_status;
	int		error_status;

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




//_________________________previous version________________________________

// #include "minishell.h"

// /**
//  * Skips leading whitespace and determines the sign of the number.
//  *
//  * @param str The input string.
//  * @param sign Pointer to an integer where the sign will be stored.
//  * @return The pointer to the first non-whitespace character in the string.
//  */
// static const char *skip_whitespace_and_sign(const char *str, int *sign)
// {
// 	*sign = 1;
// 	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
// 		str++;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			*sign = -1;
// 		str++;
// 	}
// 	return (str);
// }

// /**
//  * Checks if the entire string consists of digits.
//  *
//  * @param str The input string.
//  * @return true if the string contains only digits, false otherwise.
//  */
// static bool ft_isdigit_str(const char *str)
// {
// 	while (*str)
// 	{
// 		if (!ft_isdigit(*str))
// 			return (false);
// 		str++;
// 	}
// 	return (true);
// }

// /**
//  * Converts a string to a long long integer considering the sign.
//  *
//  * @param str The input string.
//  * @param sign The sign of the number.
//  * @return The converted long long integer.
//  * Returns LLONG_MIN or LLONG_MAX on overflow.
//  */
// static long long	custom_atoll(const char *str, int sign)
// {
// 	long long result;
// 	int digit;

// 	result = 0;
// 	while (*str)
// 	{
// 		digit = *str - '0';
// 		if (result > (LLONG_MAX - digit) / 10)
// 		{
// 			if (sign == -1)
// 				return (LLONG_MIN);
// 			else
// 				return (LLONG_MAX);
// 		}
// 		result = result * 10 + digit;
// 		str++;
// 	}
// 	return (sign * result);
// }

// /**
//  * Normalizes the exit status to be within the range of 0-255.
//  *
//  * @param num The input number to normalize.
//  * @return The normalized exit status.
//  */
// static int normalize_exit_status(long long num)
// {
// 	if (num < 0)
// 		num = 256 + (num % 256);
// 	else if (num > 255)
// 		num = num % 256;
// 	return ((int)num);
// }

// /**
//  * Handles exit errors by displaying an appropriate error message.
//  *
//  * @param error_code The error code to handle.
//  * @param arg The argument causing the error.
//  * @param shell The shell context.
//  * @return The error status.
//  */
// static int handle_exit_errors(int error_code, char *arg, t_shell *shell)
// {
// 	if (error_code == 1)
// 		return (error_msg("exit: ", NULL, "too many arguments", shell));
// 	else if (error_code == 2)
// 		return (error_msg("exit: ", arg, ": numeric argument required", shell));
// 	return (EXIT_FAILURE);
// }

// /**
//  * Checks for special exit cases such as no arguments or the "--" argument.
//  *
//  * @param cmd The command structure containing arguments.
//  * @param shell The shell context.
//  * @param error_status Pointer to store the error status.
//  * @return 0 if special cases are handled, -1 otherwise.
//  */
// static int check_special_exit_cases(t_cmd *cmd, t_shell *shell, int *error_status)
// {
// 	if (!cmd->cmd_args[1])
// 		return (0);
// 	if (ft_strcmp(cmd->cmd_args[1], "--") == 0)
// 	{
// 		if (cmd->cmd_args[2])
// 		{
// 			*error_status = 1;
// 			return (handle_exit_errors(1, NULL, shell), 1);
// 		}
// 		return (0);
// 	}
// 	return (-1);
// }

// /**
//  * Checks if the provided argument is a valid numeric argument.
//  *
//  * @param arg The argument to check.
//  * @param num Pointer to store the converted number.
//  * @param shell The shell context.
//  * @param error_status Pointer to store the error status.
//  * @return 2 if the argument is invalid, -1 otherwise.
//  */
// static int check_numeric_argument(const char *arg, long long *num, t_shell *shell, int *error_status)
// {
// 	int sign;
// 	const char *str;

// 	str = skip_whitespace_and_sign(arg, &sign);
// 	if (*str == '\0' || !ft_isdigit_str(str))
// 	{
// 		*error_status = 2;
// 		return (handle_exit_errors(2, (char *)arg, shell), 2);
// 	}
// 	*num = custom_atoll(str, sign);
// 	if (*num == LLONG_MIN || *num == LLONG_MAX)
// 	{
// 		*error_status = 2;
// 		return (handle_exit_errors(2, (char *)arg, shell), 2);
// 	}
// 	return (-1);
// }

// /**
//  * Checks if there are additional arguments after the numeric argument.
//  *
//  * @param cmd The command structure containing arguments.
//  * @param shell The shell context.
//  * @param error_status Pointer to store the error status.
//  * @return 1 if there are additional arguments, -1 otherwise.
//  */
// static int check_additional_arguments(t_cmd *cmd, t_shell *shell, int *error_status)
// {
// 	if (cmd->cmd_args[2])
// 	{
// 		*error_status = 1;
// 		return (handle_exit_errors(1, NULL, shell), 1);
// 	}
// 	return (-1);
// }

// /**
//  * Parses the exit arguments and determines the appropriate exit status.
//  *
//  * @param cmd The command structure containing arguments.
//  * @param num Pointer to store the converted number.
//  * @param shell The shell context.
//  * @param error_status Pointer to store the error status.
//  * @return The normalized exit status.
//  */
// static int parse_exit_arguments(t_cmd *cmd, long long *num, t_shell *shell, int *error_status)
// {
// 	int status;

// 	*num = 0;
// 	*error_status = 0;

// 	status = check_special_exit_cases(cmd, shell, error_status);
// 	if (status != -1)
// 		return (status);

// 	status = check_numeric_argument(cmd->cmd_args[1], num, shell, error_status);
// 	if (status != -1)
// 		return (status);

// 	status = check_additional_arguments(cmd, shell, error_status);
// 	if (status != -1)
// 		return (status);

// 	return (normalize_exit_status(*num));
// }

// /**
//  * The main exit function for the shell, handling the exit command logic.
//  *
//  * @param cmd The command structure containing arguments.
//  * @param shell The shell context.
//  * @return The exit status if there are too many arguments, otherwise exits the shell.
//  */
// int ft_exit(t_cmd *cmd, t_shell *shell)
// {
// 	long long num;
// 	int exit_status;
// 	int error_status;

// 	exit_status = parse_exit_arguments(cmd, &num, shell, &error_status);
// 	if (error_status == 1)
// 	{
// 		return (exit_status);
// 	}
// 	if (error_status == 2)
// 	{
// 		clean_exit(2, shell);
// 		exit(2);
// 	}
// 	clean_exit(exit_status, shell);
// 	exit(exit_status);
// }
