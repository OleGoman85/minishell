/////////  CD ///////

#include <stdio.h>
#include <unistd.h> // Для chdir и getcwd

// int ft_cd(char *path)
// {
// 	char buff[1024];

// 	if (getcwd(buff, 100) == NULL)
// 	{
// 		printf("Get Current working directory failed. \n");
// 		return (1);
// 	}

// 	// Печать текущего рабочего каталога
// 	printf("Before chdir: %s\n", getcwd(buff, 1024));

// 	// Попытка перейти в указанный каталог
// 	if (chdir(path) == -1)
// 	{
// 		perror("chdir current working directory failed.\n"); // Если возникла ошибка, выводим ее
// 		return 1;		 // Возвращаем код ошибки
// 	}

// 	// Печать текущего рабочего каталога после chdir
// 	printf("After chdir: %s\n", getcwd(buff, 100));

// 	return 0; // Возвращаем успешный код завершения
// }

// int main(int ac, char **av)
// {
// 	if (ac != 2)
// 	{
// 		printf("Usage: %s <directory>\n", av[0]);
// 		return 1;
// 	}
// 	ft_cd(av[1]);
// }
// // ❯ ./a.out eval/gnl_oleg/
// // Before chdir: /Users/ogoman/Desktop
// // After chdir: /Users/ogoman/Desktop/eval/gnl_oleg

int ft_cd(t_data *data)
{
    char buff[1024];

    if (getcwd(buff, sizeof(buff)) == NULL)
    {
        printf("Get Current working directory failed. \n");
        return (1);
    }

    printf("Before chdir: %s\n", buff); // eto skorej vsego liwnee

    if (chdir(data->av[1]) == -1)
    {
        perror("chdir current working directory failed.\n");
        return (1);
    }

    if (getcwd(buff, sizeof(buff)) != NULL)
        printf("After chdir: %s\n", buff);
    else
        perror("getcwd failed");

    return (0);
}

// Эта функция изменяет текущий рабочий каталог на тот, который 
// указан в аргументах командной строки (data->av[1]). Она использует
//  функции getcwd() для получения текущего каталога до и после 
//  изменения и chdir() для фатического изменения каталога. 
//  В случае ошибок она выводит соответствующие сообщения.