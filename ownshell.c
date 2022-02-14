#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void displaydstr(char **str);
int ft_strlen(char *str);
int cd_function(char **argv, char **env);
int escape_vir(char **argv, int ind);

void ft_del_cmd(char ***cmd)
{
    int i;
    int j;
    char **arg;

    i = 0;
    while(cmd[i] != NULL)
    {
        j = 0;
        arg = cmd[i];
        while (arg[j] != NULL)
        {
            free(arg[j]);
            arg[j] = NULL;
            j++;
        }
        free(arg);
        arg = NULL;
        i++;
    }
    free(cmd);
    cmd = NULL;
}

void error_exe(char *txt, char *arg)
{
    int len;
    int len1;
    int len2;
    char *msg;
    len1 = ft_strlen(txt);
    len2 = ft_strlen(arg);
    len = len1 + len2 + 2;
    msg = (char *)malloc(len * sizeof(char));

    int i;

    i = 0;
    while (i < len1)
    {
        msg[i] = txt[i];
        i++;
    }
    //msg[i] = '\0';
    //printf("%s", msg);

    int j;

    j = 0;
    while (j < len2)
    {
        msg[i+j] = arg[j];
        j++;
    }
    msg[i + j] = '\n';
    msg[i + j + 1] = '\0';
    write(2, msg, len);
    free(msg);
    msg = NULL;
    return ;
}

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

int ft_cmp(char *s1, char *s2)
{
    int len1;
    int len2;

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);

    if (len1 != len2)
        return (0);
    if (strncmp(s1, s2, len1) == 0)
        return (1);
    return (0);
}

char *ft_strdup(char *str)
{
    int len;
    int i;
    char *res;


    len = ft_strlen(str);
    i = 0;
    res = (char *)malloc(sizeof(char) * (len + 1));
    if (res == NULL)
        return (NULL);
    while (i < len)
    {
        res[i] = str[i];
        i++;
    }
    res[len] = '\0';
    return (res);
}

char **ft_upcmd(char **argv, int deb, int end)
{
    int i;
    char **res;
    int ind;


    i = deb;
    ind = 0;
    res = (char **)malloc(sizeof(char *) * (end - deb + 1));

    //printf("%d %d\n", deb, end);
    while (i < end)
    {
        res[ind] = ft_strdup(argv[i]);
      //  printf("%s\n", argv[i]);
        i++;
        ind++;
    }
    res[ind] = NULL;
    //displaydstr(res);

    return(res);
}

int nb_cmd(char **argv)
{
    int i;
    int res;

    i = 0;
    res = 0;
    while (argv[i] != NULL && ft_cmp(argv[i], ";") != 1)
    {
        if (ft_cmp(argv[i], "|"))
            res++;
        //printf("%d %s", i, argv[i]);
        i++;
    }
    return (res + 1);
}

int nxt_pipe(char **argv, int ind)
{
    int i;

    i = ind;

    while (argv[i] != NULL && ft_cmp(argv[i], "|") != 1 && ft_cmp(argv[i], ";") != 1)
    {
        i++;
    }
    return (i);
}

char ***load_cmd(char **argv)
{
    int nb;
    char ***res;
    int i;
    int nxt;
    int ind;


    if (argv[0] == NULL)
        return(NULL);
    ind = 0;
    nxt = 0;
    i = 0;
    nb = nb_cmd(argv);
    res = (char ***)malloc(sizeof(char **) * (nb + 1));
    while (i < nb)
    {
        nxt = nxt_pipe(argv, ind);
       // printf("%d %d\n", ind, nxt);
        res[i] = ft_upcmd(argv, ind, nxt);
        ind = nxt + 1;
        i++;
    }
    res[i] = NULL;
    return (res);
}


void displaydstr(char **str)
{
    int i;

    i = 0;
    while (str[i] != NULL)
    {
        dprintf(2, "-%s-", str[i]);
        i++;
    }
    dprintf(2, "\n");
    return ;
}

int execute(char ***cmd, char **env, int nb)
{
    int tmpin= dup(0);
    int tmpout = dup(1);
    int status;
    int fdin;

    fdin = dup(tmpin);

    int ret;
    int fdpipe[2];
    int fdout;

    for (int i = 0 ; i < nb ; i++)
    {
        if (ft_cmp(*(cmd + i)[0], "cd") == 1 && nb == 1)
        {
            if (cd_function(*(cmd + i), env))
                exit(1);
        }
        dup2(fdin, 0);
        close(fdin);
        if (i == nb - 1)
        {
            fdout = dup (tmpout);
        }
        else
        {
            pipe(fdpipe);
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }
        dup2(fdout, 1);
        close(fdout);

        ret = fork();
        if (ret == 0)
        {
            if (ft_cmp(*(cmd + i)[0], "cd") == 1)
            {
                if (cd_function(*(cmd + i), env))
                    exit(1);
                exit(0);
            }
            else
            {
                //dprintf(2, " {%s}\n", (*(cmd + i))[1]);
                execve((*(cmd + i))[0], *(cmd + i), env);
                error_exe("error: cannot execute ", (*(cmd + i))[0]);
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
    dup2(tmpout, 1);
    close(tmpin);
    close(tmpout);

    return(ret);

}

void displaycmd(char ***cmd)
{
    int i;

    i = 0;
    while (cmd[i] != NULL)
    {
        displaydstr(cmd[i]);
        i++;
    }
}

int nb_end(char **argv)
{
    int i;
    int res;

    if (argv[0] == 0)
        return (0);
    i = 0;
    res = 0;

    i = escape_vir(argv, i);
    if (argv[i] == NULL)
        return (0);
    while (argv[i] != NULL)
    {
        if (ft_cmp(argv[i], ";"))
        {
            res++;
            i = escape_vir(argv, i);
        }
        else
        {
            i++;
        }
    }
    return (res + 1);
}


int next_virg(char **argv, int ind)
{
    int i;

    i = ind;

    while (argv[i] != NULL && ft_cmp(argv[i], ";") != 1)
    {
        i++;
    }
    return (escape_vir(argv, i) - 1);
}

int len_cd(char **argv)
{
    int i = 0;

    while (argv[i] != NULL)
    {
        i++;
    }
    return (i);
}


int escape_vir(char **argv, int ind)
{
    while (argv[ind] != NULL && ft_cmp(argv[ind], ";") == 1)
    {
        ind++;
    }
    return(ind);
}

int cd_function(char **argv, char **env)
{
    int ret;
    if (len_cd(argv) != 2)
    {
        write(2, "error: cd: bad arguments\n", 25);
        return (1);
    }

    ret = chdir(argv[1]);

    if (ret == -1)
    {
        error_exe("error: cd: cannot change directory to ", argv[1]);
        return (1);
    }
    return (0);
}

int main(int argc, char **argv, char **env)
{
    char ***cmd;

    int nb;
    int tot_nb;
    int i;
    int cur;
    int status;
    int ret;
    i = 0;
    cur = escape_vir(argv, 1);
    tot_nb = nb_end(argv + 1);
    if (tot_nb == 0)
        return (0);
    while (i < tot_nb)
    {
        if (cur == argc)
            break;
        //printf("%d %d\n", cur, argc);
        nb = nb_cmd(argv + cur);
        cmd = load_cmd(argv + cur);
        //displaycmd(cmd);
        //exit(1);
        ret = execute(cmd, env, nb);
        /*
        while (waitpid(-1, &status, 0) != -1)
        {
        }
        */
        ft_del_cmd(cmd);
        cur = next_virg(argv, cur) + 1;
        i++;
    }



    /*
    char *echo1[] = {"/bin/echo", "salut1", NULL};
    char *echo2[] = {"/bin/echo", "salut2", NULL};
    char *echo3[] = {"/bin/echo", "salut3", NULL};
    char *cat[] = {"/usr/bin/cat", "f1", NULL};
    char *cat0[] = {"/usr/bin/cat", NULL};
    char *grep[] = {"/usr/bin/grep", "0", NULL};
    char *ls[] = {"/usr/bin/ls", NULL};
    //char **cmd[] = {echo1, cat, grep, NULL};
    char **cmd[] = {cat0, cat0, ls, NULL};
    */
    return (ret);
}