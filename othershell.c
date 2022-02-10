#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void execute(char ***cmd, char **env)
{
    int tmpin;
    int tmpout;
    int fdin;
    int fdout;
    int fdpipe[2][2];
    int ret;
    int status;

    tmpin = dup(0);
    tmpout = dup(1);

    fdin = dup(tmpin);

    for (int i = 0 ; i < 3 ; i++)
    {
        dup2(fdin, 0);
        close(fdin);
        if (i == 2)
        {
            fdout = dup(tmpout);
        }
        else
        {
            pipe(fdpipe[1]);
            pipe(fdpipe[0]);
            fdin = fdpipe[0][0];
            close (fdpipe[0][1]);
            fdout = fdpipe[1][1];
            close(fdpipe[1][0]);
        }
        dup2(fdout, 1);
        close(fdout);

        ret = fork();
        if (ret == 0)
        {
            execve((*(cmd + i))[0], *(cmd + i), env);
            perror("execve");
            exit(1);
        } 
    }
    dup2(tmpin, 0);
    dup2(tmpout, 1);
    close(tmpin);
    close(tmpout);
    waitpid(ret, &status, 0);


}



int main(int argc, char **argv, char **env)
{
    char *echo1[] = {"/bin/echo", "salut1", NULL};
    char *echo2[] = {"/bin/echo", "salut2", NULL};
    char *echo3[] = {"/bin/echo", "salut3", NULL};
    char *cat[] = {"/usr/bin/cat", "f1", NULL};
    char *grep[] = {"/usr/bin/grep", "0", NULL};
    char *ls[] = {"/usr/bin/ls", NULL};
    char **cmd[] = {echo1, cat, grep, NULL};

    execute(cmd, env);
    return (0);
}