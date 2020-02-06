#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(int argc, char* argv[])
{
	int fd[2];
	char buf[4096];
	int pr = pipe(fd);
	if(pr < 0)
	{
		perror("pipe error");
		exit(1);
	}
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)
	{
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);
		int er = execlp("ls", "ls", "-l", NULL);
		if(er < 0)
		{
			perror("execlp error");
			exit(1);
		}
	}
	else
	{
		//sleep(1);
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		int er = execlp("wc", "ls", "-l", NULL);
		if(er < 0)
		{
			perror("execlp error");
			exit(1);
		}
		close(fd[0]);
		wait(NULL);
	}
	return 0;
}

