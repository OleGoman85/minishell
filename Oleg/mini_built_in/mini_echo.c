///////////// ECHO /////////////

#include <stdio.h>
#include <string.h>

int ft_strcmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return 0;

    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int ft_echo(int arg_num, char **str)
{
    int i = 1;
    int newline = 1;  // Переменная для отслеживания, нужно ли добавлять новую строку
    while (i < arg_num)
    {
        if (ft_strcmp(str[i], "-n") == 0)
            newline = 0; // Если найдена опция -n, не добавляем новую строку
        else
        {
            printf("%s", str[i]);
            if (i != arg_num - 1)
                printf(" ");
        }
        i++;
    }
    if (newline) // Добавляем новую строку, если -n не было
    {
        printf("\n");
    }
    return 0;
}

int main(int argc, char **argv)
{
    ft_echo(argc, argv);
    return 0;
}

// ❯ ./a.out -n "privet"
// privet%
// ❯ ./a.out "privet"
// privet