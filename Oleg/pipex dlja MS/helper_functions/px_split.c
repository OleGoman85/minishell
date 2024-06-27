/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:53:39 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:27:21 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	px_word_len(const char *str, char separator);
static int	px_count_word(char const *str, char separator);
static char	*px_free_array(char **array, int n);
char		**px_split(char const *str, char c);

static char	*px_free_array(char **array, int n)
{
	int	i;

	i = n;
	while (i > 0)
		free(array[--i]);
	free(array);
	return (NULL);
}

static int	px_count_word(char const *str, char separator)
{
	int	word_count;
	int	current_word_length;

	word_count = 0;
	while (*str)
	{
		current_word_length = px_word_len(str, separator);
		if (current_word_length != 0)
		{
			word_count++;
			str += current_word_length;
		}
		else
			str++;
	}
	return (word_count);
}

static int	px_word_len(const char *str, char separator)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != separator)
		i++;
	return (i);
}

char	**px_split(char const *str, char c)
{
	char	**array;
	int		word;
	int		i;
	int		current_word_length;

	word = px_count_word(str, c);
	array = (char **) malloc((word + 1) * sizeof (char *));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < word)
	{
		current_word_length = px_word_len(str, c);
		if (current_word_length != 0)
		{
			array[i] = px_substr(str, 0, current_word_length);
			if (array[i] == NULL)
				return ((char **) px_free_array(array, i));
			i++;
		}
		str += (current_word_length + 1);
	}
	array[i] = NULL;
	return (array);
}
