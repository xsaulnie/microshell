#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void putstr_fd(int fd, char *str);

int ft_strlen(char *str)
{
    int i;

    i = 0;

    while (str[i] != '\0')
        i++;
    return (i);
}

void exit_fatal()
{
    putstr_fd(2, "error: exit fatal\n");
    exit(1);
}

int print_error(char *msg, char *str)
{
    int len1 = ft_strlen(msg);
    int len2 = ft_strlen(str);

    char *buf;

    int tot_len = len1 + len2 + 1;

    buf = malloc(sizeof(char) * (tot_len + 1));
    if (buf == NULL)
        return exit_fatal(), 0;
    int i = 0;

    while (i < len1)
    {
        buf[i] = msg[i];
        i++;
    }
    while (i < tot_len - 1)
    {
        buf[i] = str[i];
        i++;
    }
    buf[tot_len - 1] = '\n';
    buf[tot_len] = '\0';
    write(2, buf, ft_strlen(buf));
    return 0;
}

void putstr_fd(int fd, char *str)
{
    write(fd, str, ft_strlen(str));
}

int cmp(char *str1, char *str2)
{
    int len1 = ft_strlen(str1);
    int len2 = ft_strlen(str2);

    if (len1 != len2)
        return 0;
    if (strncmp(str1, str2, len1) == 0)
        return (1);
    return (0);
}

char *ft_strdup(char *str)
{
    char *res;
    int len = ft_strlen(str);

    res = malloc(sizeof(char) * len);
    if (res == NULL)
        return exit_fatal(), NULL;
    for (int i = 0 ; i < len ; i++)
    {
        res[i] = str[i];
    }
    str[len] = '\0';
    return (res);
}

char **ft_cmd(char **argv, char deb, char end)
{
    int len = end - deb;

    char **res;

    res = malloc(sizeof(char*) * (len + 1));
    if (res == NULL)
        return exit_fatal(), NULL;
    for (int i = 0 ; i < len ; i++)
    {
        res[i] = ft_strdup(argv[i + deb]);
    }
    res[len] = NULL;
    return (res);   
}

int nb_cmd(char **argv)
{
    int i = 0;
    int res = 0;
    while (argv[i] && cmp(argv[i], ";") != 1)
    {
        if (cmp(argv[i], "|") == 1)
            res++;
        i++;
    }
    return res + 1;
}

int next_pipe(char **argv, int *breaker)
{
    if (argv[0] == NULL)
        return (0);
    int i = 0;
    while(argv[i] && cmp(argv[i], ";") != 1)
    {
        if (cmp(argv[i], "|"))
        {
            return (i);
        }
        i++;
    }
    *breaker = 1;
    return i;
}

char ***load_cmd(char **argv)
{
    int nb;
    char ***res;
    nb = nb_cmd(argv);

    int breaker = 0;
    res = malloc(sizeof(char **) * (nb + 1));
    if (res == NULL)
        return exit_fatal(), NULL;
    int nxt;
    int i = 0;
    int cur = 0;
    while (breaker != 1)
    {
        nxt = next_pipe(argv + cur, &breaker);
        res[i] = ft_cmd(argv + cur, 0, nxt);

        i++;
        cur += nxt + 1;
    }
    res[nb] = NULL;
    return (res);

}

int escape_virg(char **argv, int cur)
{
    while (argv[cur] && cmp(argv[cur], ";") == 1)
        cur++;
    return (cur);
}

int next_virg(char **argv, int cur)
{
    while (argv[cur] && cmp(argv[cur],  ";") != 1)
    {
        cur++;
    }
    return(escape_virg(argv, cur) - 1);
}

void ft_del_cmd(char ***cmd)
{
    for (int i = 0 ; cmd[i] ; i++)
    {
        for(int j = 0 ; cmd[i][j] ; j++)
        {
            free(cmd[i][j]);
        }
        free(cmd[i]);
    }
    free(cmd);
}

void display_cmd(char ***cmd)
{
    for (int i = 0 ; cmd[i] ; i++)
    {
        for(int j = 0 ; cmd[i][j] ; j++)
        {
            printf("%s", cmd[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv, char **env)
{
    (void)(env);
    
    int cur = escape_virg(argv, 1);
    char ***cmd;

    if (argv[1] == NULL)
        return (0);
    while (cur < argc)
    {
        cmd = load_cmd(argv + cur);
       // exit(1);
        display_cmd(cmd);
        printf("\n");

        ft_del_cmd(cmd);
        cur = next_virg(argv, cur) + 1;
    }
    

    /*
    char **test;
    test = ft_cmd(argv, 1, 3);
    for (int i = 0 ; test[i] ; i++)
        printf("%s", test[i]);
    */
    return (0);
}