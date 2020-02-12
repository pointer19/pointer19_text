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
	printf("catch signal = %d\n",signum);
}
int main(int argc, char* argv[])
{
		fd[2];
		pipe(fd);
		pid_t pid;
		pid=fork();
		if(pid==0)
		{
			printf("-----%d-----\n",getpid());
			sleep(1);
		}
		else if(pid>0)
		{
			struct sigaction act,oldact;
			act.sa_handler = get_sig;
			act.sa_flags=0;
			sigemptyset(&act.sa_mask);
			sigaction(SIGINT,&act,&oldact);
			close(fd[1]);
			read(fd[0],,);
		}
	return 0;
}

