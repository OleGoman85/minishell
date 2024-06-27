//////////////// UNSET ////////////////

#include "mini.h"

int ft_strcmp(char *s1, char *s2);
void ft_unset(t_builtin *built);


// sohranjaet envp v massiv
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

// zapuskaet i vivodit envp
int main (int ac, char **av, char *envp[])
{
    (void) ac;
    int i;
    t_builtin built;

    i = 0;
    if (av[1] != NULL)
        built.variable = av[1];
    else
        printf("Error: No variable provided\n");
    built.all_env = ft_all_env(&built, envp);
    if (built.all_env == NULL)
        return (0);
    else
        ft_unset(&built);
    while (i < built.envp_size)
    {
        printf("%s\n", built.all_env[i]);
        i++;
    }
    free (built.all_env);
    return (0);
}

// udaljaet stroku iz massiva;
void ft_unset(t_builtin *built)
{
    int i = 0;
    int j;

    while (built->all_env[i] != NULL)
    {
        if (ft_strcmp(built->all_env[i], built->variable) == 0)
        {
            j = i;
            while (built->all_env[j + 1] != NULL)
            {
                built->all_env[j] = built->all_env[j + 1];
                j++;
            }
            built->all_env[j] = NULL;
            built->envp_size = built->envp_size--;
            return;
        }
        else
        {
            i++;
        }
    }
}

// sravnivaet stroki do '='
int ft_strcmp(char *s1, char *s2)
{
    int i = 0;

    while (s1[i] && s1[i] != '=' && s2[i])
    {
        if (s1[i] != s2[i])
            return 1;
        i++;
    }
    if (s1[i] == '=' && s2[i] == '\0')
        return 0;
    return 1;
}
