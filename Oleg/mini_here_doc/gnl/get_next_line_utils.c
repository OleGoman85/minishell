/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 07:31:16 by ogoman            #+#    #+#             */
/*   Updated: 2024/05/21 08:19:49 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Looks for a newline character in buffer starting from start_index.
// Returns the index of the character folowing the newline.
// Returns 0 is no newline was found.


ssize_t	find_new_string(char *buffer, ssize_t start_index)
{
	while (buffer[start_index])
	{
		if (buffer[start_index] == '\n')
			return (start_index + 1);
		start_index++;
	}
	return (0);
}

// Returns the length of the string s
ssize_t	ft_strlen(const char *s)
{
	ssize_t	res;

	res = 0;
	while (s[res])
		res++;
	return (res);
}

// Copy the old string to the new one
char	*ft_strcpy(char *result, const char *old_result)
{
	size_t	i;

	i = 0;
	while (old_result[i] != '\0')
	{
		result[i] = old_result[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

// Add part of the buffer to the new string
char	*ft_strcat(char *dest, const char *src, size_t n)
{
	size_t	dest_len;
	size_t	i;

	i = 0;
	dest_len = ft_strlen(dest);
	while (i < n && src[i] != '\0')
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}
