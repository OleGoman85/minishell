/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 07:31:34 by ogoman            #+#    #+#             */
/*   Updated: 2024/05/21 08:00:10 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "../mini_here_doc.h"


# define BUFFER_SIZE 1000


char	*get_next_line(int fd);
ssize_t	find_new_string(char *buffer, ssize_t start_index);
ssize_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src, size_t n);

#endif // GET_NEXT_LINE_H;
