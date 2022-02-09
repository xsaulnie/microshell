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
        i++;
    }
    return (i);
}

int ft_strcmp(char *str1, char *str2)
{
    int len1;
    int len2;

    len1 = ft_strlen(str1);
    len2 = ft_strlen(str2);

    if (len1 != len2)
        return (0);
    if (strncmp(str1, str2, len1) == 0)
        return (1);
    return (0);
}


char *ft_strdup(char *str)
{
    char *res;
    int len;

    len = ft_strlen(str);
    //printf("%d\n", len);
    res = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0 ; i < len ; i++)
    {
        res[i] = str[i];
    }
    res[len] = '\0';
    //printf("| %s |\n",res);
    return (res);
}

char **arg(char **argv, int *cur)
{

    int len;
    char **res;

    len = 0;
    while (argv[*cur + len] != NULL && ft_strcmp(argv[*cur + len], "|") != 1 && ft_strcmp(argv[*cur + len], ";") != 1)
    {
        len++;
    }
    res = (char **)malloc( (len + 1)* sizeof(char*));
    for (int j = 0 ; j < len ; j++)
    {
        res[j] = ft_strdup(argv[*cur + j]);
    }
    res[len] = NULL;
    *cur += len;
    return (res);
}


void ft_deldstr(char **str)
{
    int i;

    i = 0;
    while (str[i] != NULL)
    {
        free(str[i]);
        i++;
    }
    free(str);
}

void waitall(int nb)
{
    int     status;
    pid_t   pid;

    int i = 0;

    while (i < nb)
    {
        pid = wait(&status);
        i++;
    }
}

int main(int argc, char **argv, char **env)
{
    char *cargv;
    int cur = 0;

    for (int i = 0 ; i < 1 ; i++)
    {
        int tube[2];
        pid_t pid = fork()
        if (pid == -1)
        {
            return (1);
        }
        else if (pid == 0)
        {

            close(tube[1]);
            cargv = arg(argv + 1 + cur, &cur)
            execve(argv + 1 + cur, cargv, env);
            close(tube[0])
            exit(0);
        }
        else
        {
            cargv = arg(argv + 1 + cur, &cur);
            dup2(1, tube[1], O_WRONLY);
            close(1);
            execve(argv + 1, cargv, env);
            close(tube[0]);
            close(tube[1]);
        }
        waitall();
    }
    return (0);
}