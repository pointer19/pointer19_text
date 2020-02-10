#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<errno.h>
int main(int argc,char* argv[])
{
	int fd[2];
	int pr=pipe(fd);
	if(pr<0)
	{
		perror("pipe error");
		exit(1);	
	}
	pid_t pid=fork();
	if(pid<0)
	{
		perror("frok error");
		exit(1);
	}
	else if(pid==0)
	{
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls","-l",NULL);	
	}

	return 0;	
}

