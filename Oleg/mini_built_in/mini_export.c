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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mini.h"

// typedef struct s_builtin
// {
//     char    **all_env;
//     char    *variable;
//     int     envp_size;
//     char    **new_envp;
// }   t_builtin;

int check_variable(t_builtin *built);
void *ft_realloc(void *ptr, size_t old_size, size_t new_size);
char **ft_all_env(t_builtin *built, char *envp[]);
char **ft_new_envp(t_builtin *built);

//funkcija kopirovanija peremennih okruzenija v massiv ukazatelej
char **ft_all_env(t_builtin *built, char *envp[])
{
    int i;

    built->envp_size = 0;
    i = 0;
    while (envp[built->envp_size] != NULL) //kolichestvo envp
        built->envp_size++;
    built->all_env = (char **)malloc((built->envp_size + 1) * sizeof(char *)); // pamjat dlja envp
    if (built->all_env == NULL)
        return (NULL);
    while (i < built->envp_size) //stroki iz envp v all_env
    {
        built->all_env[i] = envp[i];
        i++;
    }
    built->all_env[built->envp_size] = NULL;
    return (built->all_env);
}

int main(int ac, char **av, char *envp[])
{
    (void)ac;
    t_builtin built;

    built.all_env = ft_all_env(&built, envp);
    if (built.all_env == NULL)
        return (1);
    if (av[1] != NULL)
    {
        built.variable = av[1];
        if (check_variable(&built))
        {
            built.new_envp = ft_new_envp(&built);
            if (built.new_envp == NULL)
            {
                free (built.all_env);
                return (1);
            }
            free(built.new_envp);
        }
        else
            printf("Error: Invalid variable format\n");
    }
    else
        printf("Error: No variable provided\n");
    return (0);
}

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t i;
	i = 0;
	if (new_size <= old_size)
	{
		return ptr;
	}
	void *new_ptr = malloc(new_size);
	if (new_ptr == NULL)
	{
		return NULL;
	}

	while (i < old_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}


//proverka argv
int check_variable(t_builtin *built)
{
    int i;

    i = 0;
    while (built->variable[i] != '=' && built->variable[i] != '\0') // poka ne vstretitsja = ili \0
    {
        if (!((built->variable[i] >= 'A' && built->variable[i] <= 'Z') ||
              (built->variable[i] >= 'a' && built->variable[i] <= 'z') ||
              (built->variable[i] >= '0' && built->variable[i] <= '9') ||
              (built->variable[i] == '_')))
        {
            printf("Error: Characters must be Numbers, Letters or '_'\n");
            return 0;
        }
        i++;
    }
    if (built->variable[i] == '\0') // konec stroki i net '='
    {
        printf("Error: '=' character is missing\n");
        return 0;
    }
    // uspewnoe vipolnenie
    return 1;
}

char **ft_new_envp(t_builtin *built)
{
    int j = 0;
    size_t old_size = (built->envp_size + 1) * sizeof(char *);
    size_t new_size = (built->envp_size + 2) * sizeof(char *); 
    built->new_envp = ft_realloc(built->all_env, old_size, new_size);
    if (built->new_envp == NULL)
        return (NULL); //error
    // Добавление новой переменной окружения в конец массива
    built->new_envp[built->envp_size] = built->variable;
    built->new_envp[built->envp_size + 1] = NULL; // Установка NULL в конце массива
    built->envp_size++;
    while (j < built->envp_size)
    {
        printf("%s\n", built->new_envp[j]);
        j++;
    }
    return(built->new_envp);
}