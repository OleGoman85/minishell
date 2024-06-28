//////// PWD /////////

int ft_pwd(t_data *data)
{
    char path[1024];
    if (getcwd(path, sizeof(path)) != NULL)
    {
        printf("%s\n", path);
    }
    else
    {
        perror("pwd error");
        return 1;
    }
    return 0;
}

// Эта функция выводит текущий рабочий каталог, используя функцию getcwd().