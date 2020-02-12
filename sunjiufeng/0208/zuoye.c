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

void catch_child(int signum)
{
	pid_t wpid;
	while((wpid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("wpid = %d\n", wpid);
	}
}

int main(int argc, char* argv[])
{
	pid_t pid;
	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGCHLD);

	sigprocmask(SIG_BLOCK, &set, NULL);

	int i = 0;
	for(; i < 10; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			sys_err("fork error");
		}
		else if(pid == 0)
		{
			break;
		}
	}
	if(i == 10)
	{
		struct sigaction act, oldact;
		act.sa_handler = catch_child;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);

		sigaction(SIGCHLD, &act, &oldact);
		sigprocmask(SIG_UNBLOCK, &set, NULL);
		
		printf("----parent pid = %d----\n", getpid());
		int n = 0;
		while(1)
		{
			printf("*********\n");
			sleep(1);
		}
	}
	else
	{
		printf("child pid = %d\n", getpid());
	}

	return 0;
}

