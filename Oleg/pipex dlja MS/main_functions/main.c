/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:50:53 by ogoman            #+#    #+#             */
/*   Updated: 2024/07/02 10:32:07 by ogoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
#include <string.h>

// int	main(int ac, char **av, char **env)
// {
// 	t_data	data;

// 	if (ac >= 2)
// 	{
		
// 		// data.av = &av[1];
// 		// data.env = env;
// 		processes_making(&data);
// 	}
// 	else
// 		main_errors(3);
// 	return (WEXITSTATUS(data.status));
// }


////////minishell/////////


int pipex(t_data *data)
{
    data->cmd_index = 0;
    create_pipes(data);
    while (data->cmd_index < data->num_cmds)
    {
        data->pid[data->cmd_index] = fork();
        if (data->pid[data->cmd_index] < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        handle_child_process(data);
        data->cmd_index++;
    }
    close_pipes_fd(&data);
    free_pipes(&data);
    free(data->pid);
    data->cmd_index = 0;
    while(data->cmd_index < data->num_cmds)     // Ожидание завершения всех дочерних процессов
    {
        waitpid(data->pid[data->cmd_index], &data->status, 0);
        data->cmd_index++;
    }
    return (WEXITSTATUS(data->status));
}

// podfunkcija dlja obrabotki dochernih processov
void handle_child_process(t_data *data)
{
    if (data->pid[data->cmd_index] < 0)
        main_errors(2);
    else if (data->pid[data->cmd_index] == 0)
    {
        execute_process(data);
        exit(EXIT_SUCCESS); // Добавлено завершение дочернего процесса
    }
}