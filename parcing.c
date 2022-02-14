#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h> // un s

int ft_strlen(char *str)
{
    int i;

    i = 0;

    while (str[i] != '\0')
    {
        i++;
    }
    return (i);
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void ft_putstr_fd(int fd, char *str)
{
    int len;
    len = ft_strlen(str);

    write(fd, str, len);
}

void exit_fatal()
{
    ft_putstr_fd(2, "error exit fatal\n");
    exit(1);
}

int ft_cmp(char *str1, char *str2)
{
    int len1 = ft_strlen(str1);
    int len2 = ft_strlen(str2);

    if (len1 != len2)
        return (0);
    if (strncmp(str1, str2, len1) == 0) // 0 !
        return (1);
    return (0);
}

char *ft_strdup(char *str)
{
    char *res;
    int len = ft_strlen(str);
    res = (char *)malloc(sizeof(char) * (len + 1));
    if (res == NULL)
    {
        exit_fatal();
        return (NULL);
    }
    for (int i = 0 ; i < len ; i++)
    {
        res[i] = str[i];
    }
    res[len] = '\0';
    return (res);
}

char **ft_cmd(char **argv, int deb, int end)
{
    char **res;
    int len;

    len = end - deb + 1;

    res = (char **)malloc(sizeof(char *) * len);
    if (res == NULL)
    {
        return exit_fatal(), (NULL);
    }

    for (int i = deb ; i < end ; i++)
    {
        res[i - deb] = ft_strdup(argv[i]); // - deb !!!
    }
    res[len] = NULL;
    return (res);
}

int escape_vir(char **argv, int ind)
{
    while(argv[ind] != NULL && ft_cmp(argv[ind], ";" ) == 1)
    {
        ind++;
    }
    return (ind);
}

int nb_cmd(char **argv, int ind)
{
    int res = 0;

    if (argv[0] == NULL)
        return (0);
    ind = escape_vir(argv, ind);
    if (argv[ind] == NULL)
        return (0);
    while(argv[ind] != NULL && ft_cmp(argv[ind], ";") != 1)
    {
        if (ft_cmp(argv[ind], "|") == 1)
            res++;
        ind++;
    }
    //exit(0);
    return (res + 1);
}


int next_pipe(char **argv, int cur)
{
    while(argv[cur] != NULL && ft_cmp(argv[cur], "|") != 1)
    {
        cur++;
    }
    return (cur);
}

char ***load_cmd(char **argv)
{
    char ***res;
    int ind = escape_vir(argv, 0);
    int nb = nb_cmd(argv, ind);
    int nxt;
    printf("%d\n", nb);
    if (nb == 0)
        return (NULL);
    res = (char ***)malloc(sizeof(char **) * (nb + 1));
    if (res == NULL)
    {
        exit_fatal();
        return (NULL);
    }

    for (int i = 0 ; i < nb ; i++)
    {
        nxt = next_pipe(argv, ind);
        printf("%d %d\n", ind, nxt);
        res[i] = ft_cmd(argv, ind, nxt);
        ind = nxt + 1; //oupsi
    }
    res[nb] = NULL;

    return (res);

}

int nb_allcmd(char **argv)
{
    int i = 0;
    int res = 0;
    i = escape_vir(argv, i);
    while (argv[i] != NULL)
    {
        if (ft_cmp(argv[i], ";") == 1)
        {
             i = escape_vir(argv, i);
            res++;
        }
        else
        {
            i++; //important
        }
    }
    return (res + 1);
}

void displaycmd(char ***cmd)
{
    for (int i = 0 ; cmd[i] != NULL ; i++)
    {
        for (int j = 0 ; cmd[i][j] != NULL ; j++)
        {
            printf("%s\n", cmd[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int next_virg(char **argv, int cur)
{
    while(ft_cmp(argv[cur], ";") != 1)
    {
        cur++;
    }
    return(escape_vir(argv, cur));
}

void ft_del_cmd(char ***cmd)
{
    for (int i = 0 ; cmd[i] != NULL ; i++)
    {
        for (int j = 0 ; cmd[i][j] != NULL ; j++)
        {
            free(cmd[i][j]);
        }
        free(cmd[i]);

    }
    free(cmd);
}

int main(int argc, char **argv, char **env)
{

    char ***cmd;
    int nb_all;
    int i = 0;
    int ind = 1;
    nb_all = nb_allcmd(argv + 1);
    while (i < nb_all)
    {
        if (ind == argc)
            break ;

        cmd = load_cmd(argv + ind);

        displaycmd(cmd);

                    exit(0);
        ft_del_cmd(cmd);
        ind = next_virg(argv, ind);
        i++;
    }
    return (0);
}