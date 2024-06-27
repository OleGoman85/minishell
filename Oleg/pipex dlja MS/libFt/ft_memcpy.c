/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:26:21 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 15:47:12 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*p;
	char	*s;

	if (dst == NULL && src == NULL)
	{
		return (0);
	}
	p = (char *) dst;
	s = (char *) src;
	i = 0;
	while (i < n)
	{
		p[i] = s[i];
		i++;
	}
	return (dst);
}
