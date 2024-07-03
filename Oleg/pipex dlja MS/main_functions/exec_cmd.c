/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 11:02:56 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/02 10:12:56 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void exec_cmd(t_data *data, char **cmd)
{
    if (strcmp(data->cmd_opt[data->cmd_index][0], "cd") == 0)
        ft_cd(data);
    else if (strcmp(data->cmd_opt[data->cmd_index][0], "echo") == 0)
        ft_echo(data);
    else if (strcmp(data->cmd_opt[data->cmd_index][0], "env") == 0)
        ft_env(data);
    else if (strcmp(data->cmd_opt[data->cmd_index][0], "export") == 0)
        ft_export(data);
    else if (strcmp(data->cmd_opt[data->cmd_index][0], "pwd") == 0)
        ft_pwd(data);
    else if (strcmp(data->cmd_opt[data->cmd_index][0], "unset") == 0)
        ft_unset(data);
    else
    {
        // Внешняя команда
        execve(data->cmd_opt[data->cmd_index][0], data->cmd_opt[data->cmd_index], data->env);
        perror("execve failed");
    }
    exit(EXIT_FAILURE); // Завершаем дочерний процесс, если произошла ошибка
}
