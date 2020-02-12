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
    //waipid传-1再加上while会循环检查回收任意子进程，从头开始,接下来问题：
	//但是再别人电脑还会出现子进程都死了（自己电脑可以在父加sleep）
	//导致父进程还没有注册，所以发送信号也没有用，一个也回收不了
	//系统会执行默认动作，子进程全部为僵尸态
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
		struct sigaction act;

		act.sa_handler=catch_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		
		sigaction(SIGCHLD,&act,NULL);
		printf("im parent,pid=%d\n",getpid());		
		while(1);
	}

	else
	{
		printf("im child,pid=%d,ppid=%d,i=%d\n",getpid(),getppid(),i);
	
	}
	 return 0;
}     

