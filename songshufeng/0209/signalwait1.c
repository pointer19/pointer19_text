#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>

void catch_child(int signum)
{
	pid_t wpid;
	wpid = wait(NULL);
	printf("wait child pid = %d\n", wpid);
}

int main(int argc, char* argv[])
{
	pid_t pid;
	int i;
	for(i = 0; i < 10; i++)
	{
		pid = fork();
		if(pid == 0)
		{
			break;
		}
	}
	if(i == 10)  //父进程
	{
		struct sigaction act;
		act.sa_handler = catch_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;

		sigaction(SIGCHLD, &act, NULL);

		printf("I'm parent,pid = %d\n", getpid());
		while(1);
	}
	else   //子进程
	{
		printf("child pid = %d,ppid = %d\n", getpid(), getppid());
	}

	return 0;
}
