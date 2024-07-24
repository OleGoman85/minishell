/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:18:18 by aarbenin          #+#    #+#             */
/*   Updated: 2023/11/06 09:18:21 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	words_count(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	**free_and_null(char **arr, int last_index)
{
	while (last_index >= 0)
	{
		free(arr[last_index]);
		last_index--;
	}
	free(arr);
	return (NULL);
}

static char	const	*skip_delimeter(char const *str, char c)
{
	while (*str == c && *str)
		str++;
	return (str);
}

static int	next_word_len(char const *str, char c)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != c)
		len++;
	return (len);
}

char	**ft_split(char const *str, char c)
{
	char	**arr;
	int		i;
	int		len;

	if (!str)
		return (NULL);
	arr = (char **)malloc((words_count(str, c) + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (*str)
	{
		str = skip_delimeter(str, c);
		len = next_word_len(str, c);
		if (len > 0)
		{
			arr[i] = ft_substr(str, 0, len);
			if (!arr[i])
				return (free_and_null(arr, i));
			i++;
		}
		str += len;
	}
	arr[i] = NULL;
	return (arr);
}
