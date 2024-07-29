/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarbenin <aarbenin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:24:20 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/29 07:00:11 by aarbenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

//________UTILS________//
struct dirent	*read_directory(DIR *directory, t_shell *shell);
pid_t			wait_for_child(int *status, t_shell *shell);
pid_t			create_process(t_shell *shell);
DIR				*open_directory(const char *path, t_shell *shell);
void			append_str(char ***args, char *str, t_shell *shell);
int				create_pipe(int pipe_fds[2], t_shell *shell);
int				execute_program(const char *path, char *const args[],
					char *const env[], t_shell *shell);
int				close_directory(DIR *directory, t_shell *shell);
int				open_file(const char *path, int mode_flags, mode_t permissions,
					t_shell *shell);
int				close_file(int fd, t_shell *shell);
int				get_file_status(const char *restrict file_path,
					struct stat *restrict file_stat, t_shell *shell);
int				duplicate_fd(int source_fd, int target_fd, t_shell *shell);

#endif