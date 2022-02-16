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

void print_error(char *str1, char *str2)
{
    int len1 = ft_strlen(str1);
    int len2 = ft_strlen(str2);
    int i = 0;

    int tot_len = len1 + len2 + 2;

    char *res;

    res = malloc(sizeof(char) * tot_len);
    while (i < len1)
    {
        res[i] = str1[i];
        i++;
    }
    while(i < tot_len)
    {
        res[i] = str2[i - len1];
        i++;
    }
    res[tot_len - 2] = '\n';
    res[tot_len - 1] = '\0';

    write(2, res, tot_len - 1);
    free(res);
    return ;
}


void *exit_fatal_ptr()
{
    write(2, "error: exit_fatal\n", 18);
    exit(1);
    return (NULL);
}

int exit_fatal(int st)
{
    write(2, "error: exit_fatal\n", 18);
    exit(st);
    return (st);
}

int ft_cmp(char *str1, char *str2)
{
    int len1 = ft_strlen(str1);
    int len2 = ft_strlen(str2);

    if (len1 != len2)
        return (0);
    if (strncmp(str1, str2, len1) == 0)
        return (1);
    return (0);
}

//

char *ft_strdup(char *str)
{
    char *res;
    int len = ft_strlen(str);

    res = malloc(sizeof(char) * (len + 1));
    if (res == NULL)
        return(exit_fatal_ptr());
    for (int i = 0 ; i < len ; i ++)
    {
        res[i] = str[i];
    }
    res[len] = '\0';
    return (res);
}

char **ft_cmd(char **argv, int deb, int end)
{
    char **res;

    res = malloc(sizeof(char *) * (end - deb + 1));
    if (res == NULL)
        return(exit_fatal_ptr());
    for (int i = deb ; i < end ; i++)
    {
        res[i - deb] = ft_strdup(argv[i]);
    }
    res[end - deb] = NULL;
    return (res);
}

int next_pipe(char **argv, int cur, int *breaker)
{
    while (argv[cur] != NULL && ft_cmp(argv[cur], ";") != 1)
    {
        if (ft_cmp(argv[cur], "|") == 1)
            return (cur);
        cur++;
    }
    *breaker = 1;
    return (cur);
}

int nb_cmd(char **argv, int cur)
{
    int res = 0;
    while(argv[cur] != NULL && ft_cmp(argv[cur], ";") != 1)
    {
        if (ft_cmp(argv[cur], "|") == 1)
            res++;
        cur++;
    }
    return(res + 1);
}

char ***load_cmd(char **argv)
{
    char ***res;
    int cur = 0;
    int nb = nb_cmd(argv, 0);
    int nxt;
    int i = 0;
    int breaker = 0;
    res = malloc(sizeof(char **) * (nb + 1));
    if (res == NULL)
        return (exit_fatal_ptr());
    while (breaker != 1)
    {
        nxt = next_pipe(argv, cur, &breaker);
        res[i] = ft_cmd(argv, cur, nxt);
        cur = nxt + 1;
        i++;
    }
    res[nb] = NULL;
    return (res);
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

void display_cmd(char ***cmd)
{
    for (int i = 0 ; cmd[i] ; i++)
    {
        for (int j = 0 ; cmd[i][j] ; j++)
        {
            printf("-%s-", cmd[i][j]);
        }
        printf("\n");
    }
}

int escape_vir(char **argv, int i)
{
    if (argv[0] == NULL)
        return (i);
    while (argv[i] != NULL && ft_cmp(argv[i], ";") == 1)
    {
        i++;
    }
    return (i);
}

int next_virg(char **argv, int i)
{
    while (argv[i] != NULL && ft_cmp(argv[i], ";") != 1)
    {
        i++;
    }
    return (escape_vir(argv, i) - 1);
}

int cd_function(char **argv)
{
    int ret;
    int len = 0;
    for (int i = 0 ; argv[i] ; i++)
    {
        len++;
    }
    if (len != 2)
    {
        write(2, "error: cd: Bad arguments\n", 25);
        return (1);
    }
    ret = chdir(argv[1]);
    if (ret == -1)
    {
        print_error("error: cd: cannot change directory to ", argv[1]);
        return (1);
    }
    return (0);
}

int execute(char ***cmd, int nb, char **env)
{
    int fdin;
    int fdout;

    int tmpin = dup(0);
    int tmpout = dup(1);

    int fdpipe[2];

    int ret;
    int status;

    fdin = dup(tmpin);

    for (int i = 0 ; i < nb ; i++)
    {
        if (cmd[1] == NULL && ft_cmp(*(cmd + 0)[0], "cd") == 1)
        {
            cd_function(*cmd);
            return (0);
        }
        dup2(fdin, 0);
        close(fdin);
        if (i == nb - 1)
        {
            fdout = dup(tmpout);
        }
        else
        {
            if(pipe(fdpipe))
                return(exit_fatal(1));
            fdin = fdpipe[0];
            fdout = fdpipe[1];
        }
        dup2(fdout, 1);
        close(fdout);
        ret = fork();
        if (ret < 0)
            return (exit_fatal(1));
        if (ret == 0)
        {
            if (ft_cmp("cd", (*(cmd + i))[0]) == 1)
            {
                exit(cd_function(*(cmd +i)));
            }
            else
            {
                execve(*(cmd + i)[0], *(cmd + i), env);
                print_error("error: cannot execute ", *(cmd + i)[0]);
                exit(127);
            }
        }
        waitpid(ret, &status, 0);
        if (WIFEXITED(status))
        {
            ret = WEXITSTATUS(status);
        }
    }

    dup2(tmpin, 0);
    dup2(tmpout, 0);
    close(tmpin);
    close(tmpout);

    return (ret);
}

int main(int argc, char **argv, char **env)
{


    char ***cmd;
    int i = 0;
    int cur = escape_vir(argv, 1);
    int nb;
    int ret;

    while (cur < argc)
    {
        cmd = load_cmd(argv + cur);
        nb = nb_cmd(argv, cur);
        ret = execute(cmd, nb, env);
        ft_del_cmd(cmd);
        cur = next_virg(argv, cur) + 1;
    }

    return (ret);
}