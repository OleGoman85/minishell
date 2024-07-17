/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:50:25 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/02 10:32:53 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

//vipolnenie komand

// podfunkcija dlja zakritija fajlovih deskriptorov pipe
//vipolnenie komand

// podfunkcija dlja zakritija fajlovih deskriptorov pipe
void close_unused_fds(t_data *data, int i)
{
    int j;

    j = 0;
    while(j < data->pipes_n)
    {
        if (i != 0 && j == i - 1)
            close(data->pipes_fd[j][1]); 
        else if (i != data->num_cmds - 1 && j == i)
            close(data->pipes_fd[j][0]);
        else
        {
            close(data->pipes_fd[j][0]);
            close(data->pipes_fd[j][1]);
        }
        j++;
    }
}
// podfunkcija dlja vipolnenija komand
void execute_process(t_data *data)
{
    close_unused_fds(data, data->cmd_index);
    if (data->cmd_index == 0)
        execute_command(data, data->cmd_opt[data->cmd_index], -1, data->pipes_fd[0][1]);
    else if (data->cmd_index == data->num_cmds - 1)
        execute_command(data, data->cmd_opt[data->cmd_index], data->pipes_fd[data->cmd_index - 1][0], -1);
    else
        execute_command(data, data->cmd_opt[data->cmd_index], data->pipes_fd[data->cmd_index - 1][0], data->pipes_fd[data->cmd_index][1]);
}


// funkcija dlja vipolnenija komandy v dochernem processe
void execute_command(t_data *data, char **cmd, int input_fd, int output_fd)
{
    if (input_fd != -1)
    {
        dup2(input_fd, STDIN_FILENO); // Подмена стандартного ввода
        close(input_fd);
    }
    if (output_fd != -1)
    {
        dup2(output_fd, STDOUT_FILENO); // Подмена стандартного вывода
        close(output_fd);
    }
    exec_cmd(data, cmd);
    exit(EXIT_FAILURE); // Завершаем дочерний процесс, если произошла ошибка
}