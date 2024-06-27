/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 08:28:48 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 15:47:44 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t num)
{
	unsigned char	*p;
	unsigned char	val;
	size_t			i;

	p = ptr;
	i = 0;
	val = (unsigned char) value;
	while (i < num)
	{
		p[i] = val;
		i++;
	}
	return (ptr);
}
