/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 11:47:25 by ogoman            #+#    #+#             */
/*   Updated: 2024/06/26 10:20:43 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

//Rabota s pipes:

// zakrivaet vse pipes_fd i poluchaet status zaverwenija
void close_pipes_fd(t_data *data)
{
    int status;
    int j;
    
    j = 0;
    while (j < data->pipes_n)
    {
        close(data->pipes_fd[j][0]);
        close(data->pipes_fd[j][1]);
        j++;
    }
    j = 0;
    while (j <= data->pipes_n)
    {
        if (waitpid(data->pid[j], &status, 0) > 0)
                data->status = WEXITSTATUS(status); // poluchenie statusa vihoda processa
        else
            perror("waitpid");
        j++;
    }
}

//ochistka vseh pipes
void free_pipes(t_data *data)
{
    int i = 0;
    while (i < data->pipes_n)
    {
        free(data->pipes_fd[i]);
        i++;
    }
    free(data->pipes_fd);
}

void	close_pipes(t_data *data)
{
    int i = 0;
    while ( i < 2 * (data->num_cmds - 1))
    {
        close(data->pipes_fd[i]);
        i++;
    }
}
// sozdanie pipes_fd
void create_pipes(t_data *data)
{
    int i;
    int j;

    // Выделяем память для массива пайпов
    data->pipes_fd = (int **)malloc(sizeof(int *) * data->pipes_n);
    if (!data->pipes_fd)
        main_errors(1);

    i = 0;
    while (i < data->pipes_n)
    {
        data->pipes_fd[i] = (int *)malloc(sizeof(int) * 2);
        if (!data->pipes_fd[i] || pipe(data->pipes_fd[i]) < 0) // esli proizowla owibka, osvobozdajutsja ranee allocirovannie fd
        {
            j = 0;
            while (j < i)
            {
                free(data->pipes_fd[j]);
                j++;
            }
            free(data->pipes_fd);
            main_errors(1);
        }
        i++;
    }
}

