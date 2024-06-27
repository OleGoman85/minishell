/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:03:42 by aarbenin          #+#    #+#             */
/*   Updated: 2024/04/15 11:13:14 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_atoi(const char *str)
{
	int		sign;
	long	result;
	long	previous;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		previous = result;
		result = result * 10 + (*str - '0');
		if (sign == 1 && previous > result)
			return (-1);
		if (sign == -1 && (result < 0 || (-previous) > result))
			return (0);
		str++;
	}
	return ((int)(sign * result));
}
