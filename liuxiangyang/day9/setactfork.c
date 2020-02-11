#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/wait.h>
void sys_err(char *str)
{
	perror(str);
	exit(1);
}
void catch_child(int signu)
{
	pid_t wpid;
	wpid=wait(NULL);
	printf("waitpid=%d\n",wpid);
}

int main(int argc,char* argv[])
{
	pid_t pid;
	int i;
	for( i=0;i<10;i++)
	{
		pid=fork();
		if(pid==0)
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
		printf("im parent,pid=%d\n",getpid());		
		while(1);
	}
	//程序是并发执行，子进程一起死，接受第一个后会屏蔽自身信号
	else
	{
		printf("im child,pid=%d,ppid=%d,i=%d\n",getpid(),getppid(),i);
	}

	 return 0;
}     

