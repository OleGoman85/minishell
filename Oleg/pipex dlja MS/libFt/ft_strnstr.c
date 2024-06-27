/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:31:57 by ogoman            #+#    #+#             */
/*   Updated: 2023/11/15 15:49:42 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *arr, const char *some_part, size_t len)
{
	size_t	some_part_len;

	if (arr == 0 && len == 0)
	{
		return (0);
	}
	some_part_len = ft_strlen(some_part);
	if (some_part_len == 0)
	{
		return ((char *)arr);
	}
	while (*arr && len >= some_part_len)
	{
		if (*arr == *some_part
			&& ft_strncmp(arr, some_part, some_part_len) == 0)
		{
			return ((char *)arr);
		}
		arr++;
		len--;
	}
	return (NULL);
}
