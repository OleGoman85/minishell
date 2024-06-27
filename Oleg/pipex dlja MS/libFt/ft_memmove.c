/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 16:40:46 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 15:47:37 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_memmove_forward(unsigned char *d,
		const unsigned char *s, size_t n)
{
	while (n--)
	{
		*d++ = *s++;
	}
}

static void	ft_memmove_backward(unsigned char *d,
		const unsigned char *s, size_t n)
{
	unsigned char		*dest;
	const unsigned char	*src;

	dest = d + n;
	src = s + n;
	while (n--)
	{
		*(--dest) = *(--src);
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dest;
	s = (const unsigned char *) src;
	if (d == s)
	{
		return (dest);
	}
	if (d < s)
	{
		ft_memmove_forward(d, s, n);
	}
	else
	{
		ft_memmove_backward(d, s, n);
	}
	return (dest);
}
