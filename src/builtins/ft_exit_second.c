/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_second.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:07:42 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:03:21 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Skips leading whitespace and determines the sign of the number.
 *
 * @param str The input string.
 * @param sign Pointer to an integer where the sign will be stored.
 * @return The pointer to the first non-whitespace character in the string.
 */
const char	*skip_whitespace_and_sign(const char *str, int *sign)
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
 * Handles exit errors by displaying an appropriate error message.
 *
 * @param error_code The error code to handle.
 * @param arg The argument causing the error.
 * @param shell The shell context.
 * @return The error status.
 */
int	handle_exit_errors(int error_code, char *arg, t_shell *shell)
{
	if (error_code == 1)
		return (error_msg("exit: ", NULL, "too many arguments", shell));
	else if (error_code == 2)
		return (error_msg("exit: ", arg, ": numeric argument required", shell));
	return (EXIT_FAILURE);
}
