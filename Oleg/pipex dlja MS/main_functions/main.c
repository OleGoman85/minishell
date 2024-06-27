/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ogoman <ogoman@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:50:53 by ogoman            #+#    #+#             */
/*   Updated: 2024/06/03 11:51:43 by ogoman           ###   ########.fr       */
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


int main(int argc, char **argv)
{
    int i;
    t_data data;

    i = 0;
    create_pipes(&data);
    while (i < data.num_cmds)
    {
        data.pid[i] = fork();
        handle_child_process(&data, i);
        i++;
    }
    close_pipes_fd(&data);
    free_pipes(&data);
    free(data.pid);

    i = 0;
    while(i < data.num_cmds)     // Ожидание завершения всех дочерних процессов
    {
        waitpid(data.pid[i], &data.status, 0);
        i++;
    }
    return WEXITSTATUS(data.status);
}

// podfunkcija dlja obrabotki dochernih processov
void handle_child_process(t_data *data, int i)
{
    if (data->pid[i] < 0)
        main_errors(2);
    else if (data->pid[i] == 0)
    {
        execute_process(data, i);
        exit(EXIT_SUCCESS); // Добавлено завершение дочернего процесса
    }
}