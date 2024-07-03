////////////////////// ft_export //////////////

// #include <stdio.h>
// #include <stdlib.h>

//funkcija vivodit zadannuju stroku stroku iz envp
// int ft_export(char *str) //
// {
//     int i = 0;
//     char *env_var = getenv(str);
//     if (env_var == NULL)
//     {
//         printf("No enviromment variable was found\n");
//         return (1);
//     }
//     printf("%s\n", env_var);
//     return(0);
// }

// int main(int ac, char **av)
// {
//     int i = 1;
//     ft_export(av[i]); // esli pervij argument v terminale.
//     return (0);
// }
// getenv - получает строку и возвращает указатель на значение
// или NULL, если совпадений нет.

//./a.out PATH
// PATH=/Users/ogoman/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Users/ogoman/.brew/bin



void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t i;

	i = 0;
	if (new_size <= old_size)
		return (ptr);
	void *new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);

	while (i < old_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
// Эта функция изменяет размер блока памяти, на который указывает
// ptr, с old_size на new_size. Она копирует данные из старой
// области памяти в новую и возвращает указатель на новую область
// памяти. Если выделение новой памяти не удалось, функция возвращает NULL.

int check_variable(t_data *data)
{
    int i = 0;

    while (data->variable[i] != '=' && data->variable[i] != '\0') // Пока не встретим '=' или '\0'
    {
        if (!((data->variable[i] >= 'A' && data->variable[i] <= 'Z') ||
              (data->variable[i] >= 'a' && data->variable[i] <= 'z') ||
              (data->variable[i] >= '0' && data->variable[i] <= '9') ||
              (data->variable[i] == '_')))
        {
            printf("Error: Characters must be Numbers, Letters or '_'\n");
            return (0);
        }
        i++;
    }
    if (data->variable[i] == '\0') // Конец строки и нет '='
    {
        printf("Error: '=' character is missing\n");
        return (0);
    }
    return 1; // Успешное выполнение
}

// Эта функция проверяет корректность формата переменной
// (data->variable), которая должна содержать только буквы,
// цифры или символ _, и обязательный символ =.

char **ft_new_envp(t_data *data)
{
    int j = 0;
    size_t old_size = (data->envp_size + 1) * sizeof(char *);
    size_t new_size = (data->envp_size + 2) * sizeof(char *); 

    data->new_envp = ft_realloc(data->new_envp, old_size, new_size);
    if (data->new_envp == NULL)
        return (NULL);

    data->new_envp[data->envp_size] = data->variable;
    data->new_envp[data->envp_size + 1] = NULL;
    data->envp_size++;

    while (j < data->envp_size)
    {
        printf("%s\n", data->new_envp[j]);
        j++;
    }
    return (data->new_envp);
}

// Эта функция добавляет новую переменную окружения
// (data->variable) в массив переменных окружения
// (data->new_envp). Она использует ft_realloc для
// увеличения памяти под data->new_envp, затем копирует
// строку data->variable в новую область памяти. В случае
// ошибки она выводит сообщение и освобождает память.

int ft_export(t_data *data)
{
    if (ft_all_env(data) == NULL)
        return 1;

    if (data->cmd_opt[data->cmd_index][1] != NULL)
    {
        data->variable = data->cmd_opt[data->cmd_index][1];
        if (check_variable(data))
        {
            if (ft_new_envp(data) == NULL)
            {
                free(data->new_envp);
                return (1);
            }
            free(data->new_envp);
        }
        else
            printf("Error: Invalid variable format\n");
    }
    else
        printf("Error: No variable provided\n");
    return (0);
}

// Эта функция добавляет новую переменную окружения,
// указанную в аргументах командной строки (data->av[1]),
// в массив переменных окружения (data->new_envp). Она проверяет формат
// переменной с помощью check_variable(data) и выводит сообщения об
// ошибках при необходимости.