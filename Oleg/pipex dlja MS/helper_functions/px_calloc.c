/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 08:47:34 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:24:48 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	*px_overflow(size_t count, size_t size)
{
	if (count == 0 || size == 0)
	{
		return (NULL);
	}
	if (size > SIZE_MAX / count)
	{
		return (NULL);
	}
	return ((void *)1);
}

void	*px_calloc(size_t count, size_t size)
{
	size_t	total_size;
	void	*ptr;

	px_overflow(count, size);
	total_size = count * size;
	ptr = malloc(total_size);
	if (ptr)
	{
		px_memset(ptr, 0, total_size);
	}
	else
	{
		return (NULL);
	}
	return (ptr);
}
