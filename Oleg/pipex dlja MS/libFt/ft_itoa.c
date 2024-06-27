/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 07:41:45 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 16:01:45 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_digits(long ln)
{
	int	digits;

	digits = 1;
	if (ln < 0)
	{
		digits++;
		ln = -ln;
	}
	while (ln >= 10)
	{
		digits++;
		ln = ln / 10;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	ln;

	ln = n;
	i = ft_digits(ln);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	str[i] = '\0';
	if (ln < 0)
	{
		str[0] = '-';
		ln = -ln;
	}
	while (ln >= 10)
	{
		str[i - 1] = '0' + (ln % 10);
		i--;
		ln = ln / 10;
	}
	str[i - 1] = '0' + ln;
	return (str);
}
