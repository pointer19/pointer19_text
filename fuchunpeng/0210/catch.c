/*******************************************************************************************************************************************************************************************************************************************************Mr.Fu**************************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include <signal.h>
#include<sys/wait.h>
void catch_child()
{
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("wait pid =%d\n",pid);
	}

}
			    
int main(int argc, char* argv[])
{
	pid_t pid;
	int i;
	sigset_t myset;
	sigemptyset(&myset);
	sigaddset(&myset,SIGCHLD);
	sigprocmask(SIG_BLOCK,&myset,NULL);

	for(i=0;i<10;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
		else if(pid >0)
		{
			continue;
		}
		else
		{
			perror("wrong");
			exit(1);
		}
	}
	if(i==10)
	{
		struct sigaction act;
		act.sa_handler=catch_child;
		sigemptyset(&act.sa_mask);

		act.sa_flags=0;
		sigaction(SIGCHLD,&act,NULL);
		sigprocmask(SIG_UNBLOCK,&myset,NULL);
		printf("I'm parent pid=%d\n",getpid());
		
		while(1);
	}
	else if(i<10)
	{
		sleep(2);
		printf("I'm child pid=%d\n",getpid());
	}
	return 0;
}

