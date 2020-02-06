#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char* argv[])
{
    int fd[2];
    int pi=pipe(fd);
    if(pi<0)
    {
	perror("pipe error");
	exit(1);
    }
    pid_t pid=fork();
    if(pid==0)
    {
	close(fd[0]);	
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        execlp("ls","ls","-l",NULL);
        exit(1);
    }
    else if(pid<0)
    {
	perror("fork error");
	exit(1);
    }
    else
    {
	close(fd[1]);
	dup2(fd[0],STDIN_FILENO);
	close(fd[0]);
	wait(NULL);
	execlp("wc","wc","-l",NULL);
	exit(1);
    }
    return 0;
}
