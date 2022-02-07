#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int ft_strlen(char *str)
{
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        i++
    }
    return (i);
}

bool ft_strcmp(char *str1, char *str2)
{
    int len1;
    int len2;

    len1 = ft_strlen(str1);
    len2 = ft_strlen(str2);

    if (len1 != len2)
        return (false);
    if (strncmp(str1, str2, len1) == 0)
        return (true);
    return (false);
}

int nb_arg(char **argv, int cur)
{
    while (ft_strlen)
}


char *ft_next(char *line, int *cur)
{
    int len;
    char *res;

    len = 0;
    while (line[len] != '\0' && line[len] != ' ')
    {
        len++;
    }
    res = (char *)malloc(sizeof(char) * len);

    for (int i = 0 ; i < len ; i++)
    {
        res[i] = line[i];
    }
    res[len] = '\0';
    *cur += len;
    return (res);

}

int main(int argc, char **argv, char **env)
{
    int cur;

    cur = 0;
    
    execve(argv[1], argv + 1, env);
    perror("execve");
    exit(EXIT_FAILURE);
    return (0);
}