/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 11:03:39 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:28:30 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	px_bzero(void *s, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = s;
	i = 0;
	while (i < n)
	{
		p[i] = 0;
		i++;
	}
}
