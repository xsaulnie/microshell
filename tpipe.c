#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void pipeline(char ***cmd, char **env)
{
    int status;
	int fd[2];
	pid_t pid;
	int fdd = 0;				

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
			}
			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			exit(1);
		}
		else {
			waitpid(-1, &status, 0); 		
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
	pipeline(cmd, env);

    return (0);
}