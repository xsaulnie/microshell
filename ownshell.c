#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void displaydstr(char **str)
{
    int i;

    i = 0;
    while (str[i] != NULL)
    {
        dprintf(2, "%s ", str[i]);
        i++;
    }
    return ;
}

void execute(char ***cmd, char **env)
{
    int tmpin= dup(0);
    int tmpout = dup(1);
    int status;
    int fdin;

    fdin = dup(tmpin);

    int ret;
    int fdpipe[2];
    int fdout;

    for (int i = 0 ; i < 3 ; i++)
    {
        dup2(fdin, 0);
        close(fdin);
        if (i == 2)
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
            //displaydstr(*(cmd + i));
            //dprintf(2, "{%s]", (*(cmd + i))[0]);
            execve((*(cmd + i))[0], *(cmd + i), env);
            perror("execve");
            exit(1);
        }
       // waitpid(ret, &status, 0);

    }
    dup2(tmpin, 0);
    dup2(tmpout, 1);
    close(tmpin);
    close(tmpout);

    waitpid(ret, &status, 0);
waitpid(ret, &status, 0);
}

int main(int argc, char **argv, char **env)
{
    char *echo1[] = {"/bin/echo", "salut1", NULL};
    char *echo2[] = {"/bin/echo", "salut2", NULL};
    char *echo3[] = {"/bin/echo", "salut3", NULL};
    char *cat[] = {"/usr/bin/cat", "f1", NULL};
    char *cat0[] = {"/usr/bin/cat", NULL};
    char *grep[] = {"/usr/bin/grep", "0", NULL};
    char *ls[] = {"/usr/bin/ls", NULL};
    //char **cmd[] = {echo1, cat, grep, NULL};
    char **cmd[] = {cat0, cat0, ls, NULL};
    execute(cmd, env);
    return (0);
}