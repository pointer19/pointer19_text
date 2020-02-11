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
//	while((wpid=waitpid(-1,NULL,WNOHANG))>0)
//一下回收函数会阻塞，不让系统接着进行，会在图中第四步等待
//最好的就是用上面的非阻塞循环回收，让夫进程该干嘛干嘛，有死了的再回收
    while((wpid=wait(NULL))!=-1)
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
		
		while(1)
		{
			printf("..........\n");
			sleep(1);
		}
	}

	else
	{
		printf("im child,pid=%d,ppid=%d,i=%d\n",getpid(),getppid(),i);
		sleep(i);
	}
	 if(i==9)
	 {
		sleep(9);
	}
	 return 0;
}     

