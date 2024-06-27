/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 11:10:49 by aarbenin          #+#    #+#             */
/*   Updated: 2023/12/11 11:10:56 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_parse(const char *format, va_list args)
{
	if (*format == 'c')
		return (ft_putchar(va_arg(args, int)));
	if (*format == 's')
		return (ft_putstr(va_arg(args, char *)));
	if (*format == 'p')
		return (ft_putptr(va_arg(args, void *)));
	if (*format == 'd' || *format == 'i')
		return (ft_putnbr(va_arg(args, int)));
	if (*format == 'u')
		return (ft_putnbr_base(va_arg(args, unsigned int), "0123456789"));
	if (*format == 'x')
		return (ft_putnbr_base(va_arg(args, unsigned int), "0123456789abcdef"));
	if (*format == 'X')
		return (ft_putnbr_base(va_arg(args, unsigned int), "0123456789ABCDEF"));
	if (*format == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		total;
	int		count;

	total = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			count = ft_parse(++format, args);
			if (count < 0)
				return (-1);
			total += count;
			format++;
		}
		else
		{
			if (ft_putchar(*format++) < 0)
				return (-1);
			total ++;
		}
	}
	va_end(args);
	return (total);
}
