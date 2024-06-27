//////// PWD /////////

#include <stdio.h>
#include <unistd.h>

int ft_pwd(void)
{
    char path[1024];
    if (getcwd(path, sizeof(path)) != NULL)
	{
        printf("%s\n", path);
    } else
	{
        printf("pwd error");
        return 1;
    }
    return 0;
}

int main (void)
{
	ft_pwd();
}