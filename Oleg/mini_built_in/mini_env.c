
/////////////// ENV ///////////

#include "mini.h"

char **ft_all_env(t_data *data)
{
    int i;

    i = 0;
    data->envp_size = 0;
    while (data->env[data->envp_size] != NULL) // Подсчёт количества строк в envp
        data->envp_size++;

    data->new_envp = (char **)malloc((data->envp_size + 1) * sizeof(char *)); // Выделение памяти для new_envp
    if (data->new_envp == NULL)
        return (NULL);

    while (i < data->envp_size) // Копирование строк из env в new_envp
    {
        data->new_envp[i] = data->env[i];
        i++;
    }
    data->new_envp[data->envp_size] = NULL;
    return (data->new_envp);
}

// Эта функция создает копию массива переменных окружения
// (data->env) в новом массиве (data->new_envp).
// Она вычисляет размер (envp_size) и выделяет достаточно
// памяти для хранения указателей на строки окружения.


int ft_env(t_data *data)
{
    int i;
    
    i = 0;
    if (ft_all_env(data) == NULL)
        return (1);

    while (i < data->envp_size)
    {
        printf("%s\n", data->new_envp[i]);
        i++;
    }
    free(data->new_envp); // Освобождение выделенной памяти

    return (0);
}

// Эта функция выводит все строки из массива переменных окружения
// (data->new_envp). Если ft_all_env(data) вернет NULL,
// функция вернет 1. После вывода она освобождает память,
// выделенную под data->new_envp.