/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:00:14 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:29:21 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

size_t	px_strlcpy(char *dst, const char *src, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (px_strlen(src));
	while (src[i] && n - 1 > i)
	{
		dst[i] = src[i];
		i++;
	}
	return (px_strlen(src));
}
