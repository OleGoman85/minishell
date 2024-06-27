/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:24:06 by aarbenin          #+#    #+#             */
/*   Updated: 2023/11/06 09:24:11 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*str;
	char	*last;
	char	ch;

	str = (char *)s;
	ch = (char)c;
	last = NULL;
	while (*str)
	{
		if (*str == ch)
			last = str;
		str++;
	}
	if (ch == '\0')
		return (str);
	return (last);
}
