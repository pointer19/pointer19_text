#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
void catch(int num)
{
	pid_t pid = waitpid(-1,NULL,WNOHANG);
	if (pid >0)
	{
		printf ("wait child %d\n",pid);
	}
}
void main()
{
	int i=0;
	pid_t pid;
	for(i=0;i<10;i++)
	{
		pid = fork();
		if(pid==0)
		{
			break;
		}
	}
	if(i==10)
	{
		struct sigaction act;
		act.sa_flags=0;
		sigemptyset(&act.sa_mask);
		//act.sa_handler = catch_sig;
		sigaction(SIGCHLD,&act,NULL);
		while(1)
		{
			sleep(1);
		}
	}
	else if(i<10)
	{
		printf("%d child,pid=%d\n",i,getpid());
		sleep(i);
	}
	return 0;
}

