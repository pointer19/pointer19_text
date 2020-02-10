#include<stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/wait.h>
void kl()
{
	wait(NULL);
}
int main(int argc,char* argv[])
{
	int pid,i;
	for(i=0;i<10;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
		if(pid<0)
		{
			perror("fork error");
			exit(1);
		}
	}
	if(i<10)
	{
		int a=getpid();
		printf("%d\n",a);
		while(1);
	}
	if(i==10)
	{
		struct sigaction act,oldact;
		act.sa_handler=kl;
		act.sa_flags=0;
		sigemptyset(&act.sa_mask);
		sigaction(SIGCHLD,&act,&oldact);
		while(1);
	}
	return 0;
}
