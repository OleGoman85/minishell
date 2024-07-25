/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 12:59:42 by aarbenin          #+#    #+#             */
/*   Updated: 2024/03/09 12:59:49 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	ft_isspace(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static double	process_fraction(const char **str)
{
	double	fraction;
	double	frac_part;

	fraction = 0.1;
	frac_part = 0.0;
	(*str)++;
	while (**str >= '0' && **str <= '9')
	{
		frac_part += (**str - '0') * fraction;
		fraction *= 0.1;
		(*str)++;
	}
	return (frac_part);
}

static double	process_integer(const char **str)
{
	double	int_part;

	int_part = 0.0;
	while (**str >= '0' && **str <= '9')
	{
		int_part = int_part * 10.0 + (**str - '0');
		(*str)++;
	}
	return (int_part);
}

static int	process_sign(const char **str)
{
	int	sign;

	sign = 1;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}

double	ft_atof(const char *str)
{
	double	result;
	int		sign;

	while (ft_isspace(*str))
		str++;
	sign = process_sign(&str);
	result = process_integer(&str);
	if (*str == '.')
		result += process_fraction(&str);
	return (sign * result);
}
