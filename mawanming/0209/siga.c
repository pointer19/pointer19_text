#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
void catch_child(int signum)
{
	pid_t wpid;
	while((wpid=waitpid(-1,NULL,WNOHANG))>0);
}
int main(int argc,char* argv[])
{
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGCHLD);
	sigprocmask(SIG_BLOCK,&set,NULL);
	pid_t pid;
	int i;
	for(i=0;i<10;i++)
	{
		pid=fork();
		if(pid<0)
		{
			perror("fork error");
			exit(1);
		}
		else if(pid==0)
		{
			break;
		}
	}
	if(i==10)
	{
		struct sigaction act;
		act.sa_handler=catch_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		sigaction(SIGCHLD,&act,NULL);
		printf("Im parent,pid=%d\n",getpid());
		while(1);
	}
	else
	{
		sleep(5);
		printf("child pid=%d\n",getpid());
	}
}
