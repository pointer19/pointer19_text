#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char* argv[])
{
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		sys_err("fork error");
	}
	else if(pid == 0)
	{
		printf("child pid = %d\n", getpid());
		printf("child gid = %d\n", getpgid(0));
		printf("child sid = %d\n", getsid(0));
		
		sleep(5);
		setsid();
		printf("+++++++++\n");

		printf("child pid = %d\n", getpid());
		printf("child gid = %d\n", getpgid(0));
		printf("child sid = %d\n", getsid(0));
	}
	else
	{
		while(1);		
	}
	return 0;
}

