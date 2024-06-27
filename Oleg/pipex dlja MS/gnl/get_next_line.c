/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:16:52 by ogoman            #+#    #+#             */
/*   Updated: 2024/05/15 11:25:43 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


int	some_error(char *str)
{
	if (str)
		free(str);
	return (-1);
}

int	get_next_line(int fd, char **line)
{
	char		*buf;
	int			bytes_read;
	static char	*remainder;

	if (fd < 0 || BUFFER_SIZE < 1 || !line)
		return (-1);
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (some_error(remainder));
	bytes_read = 1;
	while (!find_nl(remainder) && bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (some_error(buf));
		buf[bytes_read] = '\0';
		remainder = str_join(remainder, buf);
	}
	free(buf);
	*line = get_line(remainder);
	remainder = remove_up_to_newline(remainder);
	if (bytes_read == 0 && !remainder)
		return (0);
	return (1);
}