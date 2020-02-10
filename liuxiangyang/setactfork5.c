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
	sigset_t myset;
	sigemptyset(&myset);
	sigaddset(&myset, SIGCHLD);

	sigprocmask(SIG_BLOCK, &myset, NULL);
	//这是最好的方法，再没有注册之前，屏蔽掉SIGCHLD信号
	//不去处理该信号，这样子进程死之后挥发送信号，但不会递达
	//会让未决信号集该信号置1,等接触阻塞后再处理未决信号
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
		sleep(4);
		struct sigaction act;

		act.sa_handler=catch_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags=0;
		sigaction(SIGCHLD,&act,NULL);
		sigprocmask(SIG_UNBLOCK, &myset, NULL);
		printf("im parent,pid=%d\n",getpid());		
		while(1);
	}

	else
	{
		printf("im child,pid=%d,ppid=%d,i=%d\n",getpid(),getppid(),i);
	}

	 return 0;
}     

