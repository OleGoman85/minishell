/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 06:05:56 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/25 07:13:56 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

t_bltn_func	fetch_builtin_cmd(char *command);
void		init_builtins(t_bltn_cmd *commands);

int			ft_cd(t_cmd *cmd, t_shell *shell);
int			ft_echo(t_cmd *cmd, t_shell *shell);
int			ft_env(t_cmd *cmd, t_shell *shell);
int			ft_export(t_cmd *cmd, t_shell *shell);
int			ft_pwd(t_cmd *cmd, t_shell *shell);
int			ft_unset(t_cmd *cmd, t_shell *shell);
int			ft_exit(t_cmd *cmd, t_shell *sh);

#endif