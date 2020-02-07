#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>
int main(int argc,char* argv[])
{
	pid_t pid;	
	int status;
	pid=fork();
	if(pid>0)
	{
	  int wr=wait(&status);
	  if(WIFEXITED(status))
		{
			printf("zhengchangtuichu,wr=%d\n",WEXITSTATUS(status));
		}
	  else if(WIFSIGNALED(status))
	  {
		  printf("kill by signal is=%d\n",WTERMSIG(status));
	  }
		printf("wr=%d\n",wr);
		while(1);
	}
	else
	{
		printf("i am child,pid=%d\n",getpid());
		sleep(50);
		return 50;	
	}







	return 0;	
}

