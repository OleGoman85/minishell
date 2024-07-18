#include "minishell.h"

static const char	*skip_whitespace_and_sign(const char *str, int *sign)
{
	*sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

// Проверяет, состоит ли строка только из цифр
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

// Проверка, является ли символ цифрой

static long long	custom_atoll(const char *str, int sign)
{
	long long	result;
	int			digit;

	result = 0;
	while (*str)
	{
		digit = *str - '0';
		if (result > (LLONG_MAX - digit) / 10)
		{
			if (sign == -1)
				return (LLONG_MIN);
			else
				return (LLONG_MAX);
		}
		result = result * 10 + digit;
		str++;
	}
	return (sign * result);
}

static int	normalize_exit_status(long long num)
{
	if (num < 0)
		num = 256 + (num % 256);
	else if (num > 255)
		num = num % 256;
	return ((int)num);
}

static int	handle_exit_errors(int error_code, char *arg, t_shell *shell)
{
	if (error_code == 1)
		return (error_msg("exit: ", NULL, "too many arguments", shell));
	else if (error_code == 2)
		return (error_msg("exit: ", arg, ": numeric argument required", shell));
	return (EXIT_FAILURE);
}

/**
 * @brief Разбирает аргументы команды exit и определяет статус выхода.
 *
 * Эта функция обрабатывает аргументы, переданные команде `exit` в шелле.
 * Она проверяет допустимость аргументов, конвертирует числовой аргумент
 * в тип `long long`, проверяет наличие ошибок и нормализует статус выхода
 * в диапазоне от 0 до 255. В случае ошибок возвращает соответствующие
 * коды ошибок и сообщения.
 *
 * @param cmd Указатель на структуру `t_cmd`, содержащую команду и её аргументы.
 * @param num Указатель на переменную типа `long long`, в которую будет сохранён
 *            разобранный числовой аргумент.
 * @param shell Указатель на структуру `t_shell`,
	содержащую состояние и окружение шелла.
 *
 * @return Нормализованный статус выхода (0-255) в случае допустимых аргументов.
 *         Возвращает 255,
	если аргумент не является допустимой числовой строкой.
 *         Возвращает 1, если передано слишком много аргументов.
 */
static int	parse_exit_arguments(t_cmd *cmd, long long *num, t_shell *shell)
{
	int			sign;
	const char	*arg;

	*num = 0;
	if (!cmd->cmd_args[1])
		return (0);
	arg = skip_whitespace_and_sign(cmd->cmd_args[1], &sign);
	if (*arg == '\0' || !ft_isdigit_str(arg))
		return (handle_exit_errors(2, cmd->cmd_args[1], shell), 255);
	*num = custom_atoll(arg, sign);
	if (*num == LLONG_MIN || *num == LLONG_MAX)
		return (handle_exit_errors(2, cmd->cmd_args[1], shell), 255);
	if (cmd->cmd_args[2])
		return (handle_exit_errors(1, NULL, shell), 1);
	return (normalize_exit_status(*num));
}

// Основная функция exit
int	ft_exit(t_cmd *cmd, t_shell *shell)
{
	long long	num;
	int			exit_status;

	exit_status = parse_exit_arguments(cmd, &num, shell);
	clean_exit(exit_status, shell);
	exit(exit_status);
}
