
/////////////// ENV ///////////

#include "mini.h"

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
    (void) av;
    int i;
    t_builtin built;

    i = 0;
    built.all_env = ft_all_env(&built, envp);
    if (built.all_env == NULL)
        return (0);
    while (i < built.envp_size)
    {
        printf("%s\n", built.all_env[i]);
        i++;
    }
    free (built.all_env);
    return (0);
}