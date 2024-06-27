/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:24:30 by aarbenin          #+#    #+#             */
/*   Updated: 2023/11/06 09:24:36 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*start;
	const char	*end;
	char		*result;

	if (!s1)
		return (NULL);
	if (!set)
		set = "";
	while (*s1 && is_in_set(*s1, set))
		s1++;
	start = s1;
	end = start + ft_strlen(start) - 1;
	while (start <= end && is_in_set(*end, set))
		end--;
	if (start > end)
		return (ft_strdup(""));
	result = ft_substr(start, 0, end - start + 1);
	if (!result)
		return (NULL);
	return (result);
}
