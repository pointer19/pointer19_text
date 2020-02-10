#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
void sys_err(char*str)
{
	perror(str);
	exit(1);
}
void catch_sig(int signum)
{
	printf("catch signal is %d\n",signum);
}
int main()
{
	pid_t pid;
	int fd[2];
	pipe(fd);
	pid=fork();
	if(pid==0)
	{
		while(1)
		{
			printf("----child pid=%d\n",getpid());
			sleep(1);
		}
	}
	if(pid>0)
	{
		char buf[1024];
		struct sigaction act;
		act.sa_handler=catch_sig;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		sigaction(SIGCHLD,&act,NULL);
		printf("I'm parent,pid=%d\n",getpid());
		read(fd[0],buf,10);
		while(1)
		{
			printf("----------\n");
		
		}
	}
	return 0;
}

