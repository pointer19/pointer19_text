#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>




int main(int argc, char *argv[])
{
	int pipefd[2];
	pipe(pipefd); 
	pid_t pid = fork();
	int fd = open("1.txt", O_RDWR | O_CREAT, 0644);
	if(pid > 0)
	{
//		close(1);
		dup2(pipefd[1], STDOUT_FILENO);
//		dup(pipefd[1]);
//		close(pipefd[1]);
		close(pipefd[0]);
		execlp("ls","ls","-l",NULL);
		perror("execlp error:");
		exit(1);
	}
	if(pid == 0)
	{
//		close(0);
//		dup(pipefd[0]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		execlp("wc", "wc", "-l", NULL);
		exit(1);
	}



	return 0;
}

