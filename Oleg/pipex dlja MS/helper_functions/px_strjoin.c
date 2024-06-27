/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:59:16 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:29:08 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*px_strjoin(char const *s1, char const *s2)
{
	int		len;
	char	*str1;
	char	*str2;
	char	*ptr;

	str1 = (char *) s1;
	str2 = (char *) s2;
	if (!s1 || !s2)
		return (0);
	len = px_strlen(str1) + px_strlen(str2);
	ptr = px_calloc(len + 1, sizeof(char));
	while (*str1)
		*ptr++ = *str1++;
	while (*str2)
		*ptr++ = *str2++;
	return ((char *) ptr - len);
}
