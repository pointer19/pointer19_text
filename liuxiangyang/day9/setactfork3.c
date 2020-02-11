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
	while((wpid=waitpid(-1,NULL,WNOHANG))>0)
	{
		printf("waitpid=%d\n",wpid);
    }
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
		sleep(2);
		struct sigaction act;

		act.sa_handler=catch_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		
		sigaction(SIGCHLD,&act,NULL);
		printf("im parent,pid=%d\n",getpid());		
	//因为上面加了sleep，会导致子进程再没有注册前全死，信号注册后接受不到
	//信号，不会执行上面的循环回收动作，//但是我们可以通过kill给夫进程发送
	//一个信号，另启动循环回收，但这不是最优解，看下面的：

		kill(getpid(),SIGCHLD);
		while(1);
	}

	else
	{
		printf("im child,pid=%d,ppid=%d,i=%d\n",getpid(),getppid(),i);
	
	}
	 return 0;
}     

