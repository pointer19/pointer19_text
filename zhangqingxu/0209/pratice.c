#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
void get_sig(int signum)
{
	int w;
	while(w=waitpid(-1,NULL,WNOHANG))
	{
		if(w==-1)
		{
			perror("waitpid error");
			exit(1);
		}
		printf("wait child = %d\n",w);
	}
}
int main(int argc, char* argv[])
{
	int i=0;
	sigset_t myset;
	sigaddset(&myset,SIGCHLD);
	sigprocmask(SIG_BLOCK,&myset,NULL);
	for(;i<10;i++)
	{
		pid_t pid=fork();
		if(pid<0)
		{
			perror("fork error");
		}
		else if(pid==0)
		{
			break;
		}				
	}
	if(i==10)
	{
		sleep(10);
		printf("parent pid = %d\n",getpid());
		struct sigaction act,oldact;
		act.sa_handler = get_sig;
		act.sa_flags=0;
		sigemptyset(&act.sa_mask);
		sigaction(SIGCHLD,&act,&oldact);
		sigprocmask(SIG_UNBLOCK,&myset,NULL);
		while(1)
		{
			printf("---------\n");
			sleep(1);
		}
	}
	else
	{
		printf("child pid = %d\n",getpid());
	}
	if(i==9)
	{
		sleep(5);
	}
	return 0;
}

