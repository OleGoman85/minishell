/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 11:01:49 by ogoman            #+#    #+#             */
/*   Updated: 2024/03/08 07:29:28 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	px_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && i < n - 1 && s1[i])
		i++;
	return ((unsigned char ) s1[i] - (unsigned char ) s2[i]);
}
