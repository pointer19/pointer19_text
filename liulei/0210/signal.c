#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
void singal(int a)
{
	int pid;
	while(pid = waitpid(-1, NULL, WNOHANG) > 0)
	{
		printf("cpid= %d\n", pid);
	}


}


int main(int argc, char *argv[])
{

	int i;
	sigset_t sset;
	sigemptyset(&sset);
	sigaddset(&sset, SIGCHLD);
	sigprocmask(SIG_BLOCK, &sset, NULL);
	for(i=0;i<10;i++)
	{
		int pid=fork();
		if(pid==0)
		{
			break;
		}

	}
	if(i==10)
	{
		struct sigaction act;
		act.sa_handler = singal;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		sigaction(SIGCHLD, &act, NULL);
		sigprocmask(SIG_UNBLOCK, &sset, NULL);
		while(1)
		{
			printf("----\n");
			sleep(1);
		}
	}
	else
	{
	//	printf("cpid=%d",getpid());
		sleep(i);

	}

	return 0;
}

