/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 09:16:52 by ogoman            #+#    #+#             */
/*   Updated: 2024/05/21 08:20:08 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

// This function puts together a new string by adding a specific part of
//				the data we read (from the buffer) to the existing string.
//It takes care of managing memory to ensure everything fits.
static char	*append_string(char *old_res, char *buf, ssize_t start, ssize_t len)
{
	char	*result;

	result = (char *)malloc(ft_strlen(old_res) + len + 1);
	if (!result)
	{
		free(old_res);
		return (NULL);
	}
	ft_strcpy(result, old_res);
	ft_strcat(result, buf + start, len);
	free(old_res);
	return (result);
}

// Reads data from a file descriptor (fd) into a buffer (buffer)
//					with a specified size (BUFFER_SIZE).
// It updates the result string (result) dynamically
static int	read_to_buf(int fd, char **result, char *buffer)
{
	ssize_t	read_result;

	read_result = read(fd, buffer, BUFFER_SIZE);
	if (read_result == 0 && (**result))
		return (0);
	if (read_result <= 0)
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	buffer[read_result] = '\0';
	return (1);
}

// This function is used when find a complete line in data.
// It adds the remaining part of the line to result string and
//						prepares for the next round of reading.
static char	*finish_line(char *res, char *buf, ssize_t *line_start,
		ssize_t *line_len)
{
	res = append_string(res, buf, *line_start, *line_len);
	*line_start += *line_len;
	*line_len = 0;
	return (res);
}

// This function is called when no newline was found in buffer
// 		and therefore all we need to do is append buffer to result.
// It returns 0 in case or error (malloc fail), returns 1 if success.
// line_start and line_len values are also reset
//								in preparation for next buffer read.
static int	add_buf_to_res(char **res, char *buf, ssize_t *line_start,
		ssize_t *line_len)
{
	*res = append_string(*res, buf, *line_start, BUFFER_SIZE - *line_start);
	if (!*res)
		return (0);
	*line_start = 0;
	*line_len = 0;
	return (1);
}

// line_len indicates index in the buffer
//							after the end of the current line.
// line_start indicates index in the buffer from which 
//						the next line should be started to be read.
//0 value indicates that new info needs to be read to buffer.
// First obvious errors are handled
// 						and res is initialised as an empty string.
// New data is being read to buffer and added to res until either
//			newline is found or EOF is reached (or error happened)
// If newline was found, it and everything before it is added to res
//												and res is returned.
// If EOF reached and there was something in res, res is returned,
//										otherwise NULL is returned.
// The STATIC keyword means that these variables will retain their
//				values between calls to the function get_next_line.
// So, after the first call, the values of LINE_START and LINE_LEN
// 					BUFFER will be preserved, and the function will
//									resume from where it left off. 
char	*get_next_line(int fd)
{
	char			*result;
	static char		buffer[BUFFER_SIZE + 1];
	static ssize_t	string_start = 0;
	static ssize_t	string_len = 0;

	result = (char *)malloc(sizeof(char) * 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (!string_len)
	{
		if (!string_start && !read_to_buf(fd, &result, buffer))
			return (result);
		string_len = find_new_string(buffer, string_start) - string_start;
		if (string_len > 0)
			return (finish_line(result, buffer, &string_start, &string_len));
		else if (!add_buf_to_res(&result, buffer, &string_start, &string_len))
			return (NULL);
	}
	return (NULL);
}

// int main(void)
// {
// 	int	fd;

// 	fd = open("file.txt", O_RDONLY);
// 	printf("%s", get_next_line(fd));
// 	printf("%s", get_next_line(fd));
// 	close(fd);
// 	return (0);
// }