/////////  CD ///////

#include <stdio.h>
#include <unistd.h> // Для chdir и getcwd

int ft_cd(char *path)
{
	char buff[1024];

	if (getcwd(buff, 100) == NULL)
	{
		printf("Get Current working directory failed. \n");
		return (1);
	}

	// Печать текущего рабочего каталога
	printf("Before chdir: %s\n", getcwd(buff, 1024));

	// Попытка перейти в указанный каталог
	if (chdir(path) == -1)
	{
		perror("chdir current working directory failed.\n"); // Если возникла ошибка, выводим ее
		return 1;		 // Возвращаем код ошибки
	}

	// Печать текущего рабочего каталога после chdir
	printf("After chdir: %s\n", getcwd(buff, 100));

	return 0; // Возвращаем успешный код завершения
}

int main(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Usage: %s <directory>\n", av[0]);
		return 1;
	}
	ft_cd(av[1]);
}
// ❯ ./a.out eval/gnl_oleg/
// Before chdir: /Users/ogoman/Desktop
// After chdir: /Users/ogoman/Desktop/eval/gnl_oleg