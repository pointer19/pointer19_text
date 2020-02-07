#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
int main(int argc,char* argv[])
{
	pid_t pid;
	pid=fork();
	if(pid<0)
		{
			perror("error");
			exit(1);
		}
	else if(pid>0)
	{
		printf("i am father,pid=%d,ppid=%d\n",getpid(),getppid());
		sleep(5);
		return 0;	
	}
	else
	{
	while(1)
	{
		printf("I'm child pid = %d,ppid = %d\n", getpid(), getppid());
		sleep(1);
	}	
	}
	return 0;	
}

