#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


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

char **arg(char **argv)
{

    int len;
    char **res;

    len = 0;
    while (argv[len] != NULL && ft_strcmp(argv[len], "|") != 1 && ft_strcmp(argv[len], ";") != 1)
    {
        len++;
    }
    res = (char **)malloc( (len + 1)* sizeof(char*));
    for (int j = 0 ; j < len ; j++)
    {
        res[j] = ft_strdup(argv[j]);
    }
    res[len] = NULL;
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
/*
char ***getallpipe(char *argv)
{
    int nbcmd;
    char ***res;
    int lencmd;
    char **cmd;

    nbcmd = 0;
    while (argv != NULL && ft_strcmp(argv[nbcmd], ";") != 1)
    {
        if (ft_strcmp(argv[nbcmd], "|"))
        {
            nbcmd++;
        }
    }
    nbcmd++;
    res = (char ***)malloc(sizeof(char **) * (nbcmd + 1));
    for (int i = 0 ; i < nbcmd ; i++)
    {
        lencmd = 0;
        while (argv != NULL && ft_strcmp(argv[lencmd], ";") != 1 && ft_strcmp(argv[lencmd], "|" != 1))
        {
            lencmd++;
        }
        for (int j = 0 ; j < lencmd ; j++)
        {
            res[j] = (char **)(malloc(sizeof(char *) * lencmd))
            res[j][k] = ft_strdup(argv + 1);
        }        
    }


}
*/
void allpipe(char ***cmd, char **env)
{
	int fd[2];
	pid_t pid;
	int fdd = 0;				
    int status;

	while (*cmd != NULL) {
		pipe(fd);
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) {
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL) {
				dup2(fd[1], 1);
                //close(fd[1]);
			}
			close(fd[0]);
            //displaydstr(*cmd);
			execve((*cmd)[0], *cmd, env);
			exit(1);
		}
		else {
			waitpid(pid, &status, 0); 		
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
}


int main(int argc, char **argv, char **env)
{
    char *echo[] = {"/bin/echo", "salut", NULL};
    char *grep[] = {"usr/bin/grep", "0", NULL};
	char *cat[] = {"/usr/bin/cat", "f1", NULL};
    char *ls[] = {"/usr/bin/ls", NULL};
	
    char **cmd[] = {cat, grep, NULL};
	allpipe(cmd, env);

    return (0);
}