

Sdelal u sebja, chto bi ne naputat v tvoej verssi nichego i ne isportit, da i vdrug ti sama bi rewila v etoj funkcii, chto to sdelat, chto bi ne viwli owibki pri slijanii. Ne testil esli chestno na testere, no proverjal s otdelnim main.
v odnoj funkcii 26 strok, no eto fignja, komenti kogda udalim, budet 25. v etom faile 7 funkcij, mozno budet raskidat, po drugim failam, glavnoe chto bi zarabotalo


#include "minishell.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// int	ft_exit(t_cmd *cmd, t_shell *sh)
// {
// 	(void)cmd;
// 	(void)sh;
// 	clean_exit(sh->prev_cmd_status, sh);
// 	// return (EXIT_SUCCESS);
// 	exit(EXIT_SUCCESS);
// }

// Прототипы функций
static int parse_exit_arguments(t_cmd *cmd, long long *num);
static void handle_exit_errors(int error_code, const char *arg);
static int normalize_exit_status(long long num);
static bool is_digit(char c);
static bool ft_isdigit_str(const char *str);
static long long custom_atoll(const char *str, int sign);
static const char *skip_whitespace_and_sign(const char *str, int *sign);

// Основная функция exit
int ft_exit(t_cmd *cmd, t_shell *sh)
{
    long long num;
    int exit_status;

    // Парсинг аргументов команды exit
    exit_status = parse_exit_arguments(cmd, &num);

    clean_exit(sh->prev_cmd_status, sh);
    // Завершение программы с соответствующим статусом
    exit(exit_status);
}

// Парсинг аргументов команды exit
static int parse_exit_arguments(t_cmd *cmd, long long *num)
{
    int exit_status;
    int sign;

    exit_status = 0;
    *num = 0;
    // Проверяем наличие аргумента после команды exit (1)
    if (cmd->cmd_args[1] != NULL) {
        const char *arg = cmd->cmd_args[1];

        // Пропускаем начальные пробелы и проверяем знак (2)
        arg = skip_whitespace_and_sign(arg, &sign);
        // Конвертируем строку в число типа long long и проверяем, состоит ли строка только из цифр (3)
        if (!ft_isdigit_str(arg)) {
            handle_exit_errors(2, cmd->cmd_args[1]);
            return (2);
        }
        *num = custom_atoll(arg, sign);
        // Проверяем диапазон для выходного статуса (0-255)
        exit_status = normalize_exit_status(*num);
        // Проверяем наличие лишних аргументов
        if (cmd->cmd_args[2] != NULL) {
            handle_exit_errors(1, NULL);
            return (1);
        }
    }
    return (exit_status);
}

// Обработка ошибок команды exit
static void handle_exit_errors(int error_code, const char *arg)
{
    if (error_code == 1) {
        printf("exit: too many arguments\n");
    } else if (error_code == 2) {
        printf("exit: %s: numeric argument required\n", arg);
    }
}

// Нормализация статуса выхода в диапазоне 0-255
static int normalize_exit_status(long long num)
{
    if (num < 0) {
        num = 256 + (num % 256);
    } else if (num > 255) {
        num = num % 256;
    }

    return ((int)num);
}

// Конвертация строки в число типа long long без проверки пробелов и знака
static long long custom_atoll(const char *str, int sign)
{
    long long result = 0;
    int digit;

    while (*str) {
        digit = *str - '0';
        // Проверяем на переполнение
        if (result > LLONG_MAX / 10 || (result == LLONG_MAX / 10 && digit > LLONG_MAX % 10)) {
            if (sign == -1) {
                return LLONG_MIN;
            } else {
                return (LLONG_MAX);
            }
        }
        result = result * 10 + digit;
        str++;
    }

    return (sign * result);
}

// Проверка, является ли символ цифрой
static bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

// Проверяет, состоит ли строка только из цифр
static bool ft_isdigit_str(const char *str)
{
    while (*str) {
        if (!is_digit(*str)) {
            return (false);
        }
        str++;
    }
    return (true);
}

// Пропуск начальных пробелов и проверка знака числа
static const char *skip_whitespace_and_sign(const char *str, int *sign)
{
    *sign = 1;
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) {
        str++;
    }
    if (*str == '-' || *str == '+') {
        if (*str == '-') {
            *sign = -1;
        }
        str++;
    }
    return (str);
}

