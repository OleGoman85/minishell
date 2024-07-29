/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 06:05:56 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 16:17:55 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

const char	*skip_whitespace_and_sign(const char *str, int *sign);
void		init_builtins(t_bltn_cmd *commands);
int			ft_cd(t_cmd *cmd, t_shell *shell);
int			ft_echo(t_cmd *cmd, t_shell *shell);
int			ft_env(t_cmd *cmd, t_shell *shell);
int			ft_export(t_cmd *cmd, t_shell *shell);
int			ft_pwd(t_cmd *cmd, t_shell *shell);
int			ft_unset(t_cmd *cmd, t_shell *shell);
int			ft_exit(t_cmd *cmd, t_shell *sh);
int			handle_exit_errors(int error_code, char *arg, t_shell *shell);
t_bltn_func	fetch_builtin_cmd(char *command);

#endif