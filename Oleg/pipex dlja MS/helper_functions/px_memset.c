/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 08:28:48 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:28:55 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	*px_memset(void *ptr, int value, size_t num)
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
