/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:11:28 by aarbenin          #+#    #+#             */
/*   Updated: 2023/12/11 11:11:35 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	count;

	count = 0;
	if (!str)
		str = "(null)";
	while (*str)
	{
		if (ft_putchar(*str) < 0)
			return (-1);
		count++;
		str++;
	}
	return (count);
}

int	ft_putnbr_base(unsigned long long n, char *base)
{
	unsigned long long	base_len;
	int					count;

	base_len = 0;
	while (base[base_len])
		base_len++;
	count = 0;
	if (n >= base_len)
	{
		count = ft_putnbr_base(n / base_len, base);
		if (count < 0)
			return (-1);
	}
	if (ft_putchar(base[n % base_len]) < 0)
		return (-1);
	return (count + 1);
}

int	ft_putnbr(int n)
{
	unsigned int	num;
	int				count;

	count = 0;
	if (n < 0)
	{
		if (ft_putchar('-') < 0)
			return (-1);
		num = (unsigned int)(-n);
		count = 1;
	}
	else
	{
		num = (unsigned int)n;
	}
	return (count + ft_putnbr_base(num, "0123456789"));
}

int	ft_putptr(void *ptr)
{
	int	count;

	count = ft_putstr("0x");
	if (count < 0)
		return (-1);
	return (count + ft_putnbr_base((unsigned long long)ptr,
			"0123456789abcdef"));
}
