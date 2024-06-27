/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:50:25 by ogoman            #+#    #+#             */
/*   Updated: 2024/06/03 11:55:58 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

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
void execute_process(t_data *data, int i)
{
    close_unused_fds(data, i);
    if (i == 0)
        execute_command(data->cmd_opt[i], (char *[]){data->cmd_opt[i], NULL},
            data->env, -1, data->pipes_fd[0][1]);
    else if (i == data->num_cmds - 1)
        execute_command(data->cmd_opt[i], (char *[]){data->cmd_opt[i], NULL},
            data->env, data->pipes_fd[i - 1][0], -1);
    else
        execute_command(data->cmd_opt[i], (char *[]){data->cmd_opt[i], NULL},
            data->env, data->pipes_fd[i - 1][0], data->pipes_fd[i][1]);
}

// funkcija dlja vipolnenija komandy v dochernem processe
void execute_command(char *path, char **args, char **envp, int input_fd, int output_fd)
{
    if (input_fd != -1)
    {
        dup2(input_fd, STDIN_FILENO); //podmena standartnogo vvoda
        close(input_fd);
    }
    if (output_fd != -1)
    {
        dup2(output_fd, STDOUT_FILENO); //podmena standartnogo vivoda
        close(output_fd);
    }
    execve(path, args, envp); //zapusk komandi
    perror("execvp failed");
    exit(1);
}