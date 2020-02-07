#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>
int main(int argc,char* argv[])
{
	pid_t pid,wpid;
	int i;
	for(i=0;i<5;i++)
	{
		pid=fork();
		if(pid==0)
	    {
			break;
		}
		if(i==2)
		{
			wpid=pid;
		}
	}
	if(i==5)//父进程
	{
		int wr=waitpid(wpid,NULL,0);
		printf("wr=%d\n",wr);
	}
	else
	{
		sleep(5);
		sleep(i);
		printf("%dst,childpid=%d\n",i,getpid());
			
	}
	
	return 0;	
}

