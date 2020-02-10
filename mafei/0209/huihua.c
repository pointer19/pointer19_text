/**********************************************************************/
/*******************************welcome********************************/
/********************************Mr.M**********************************/
/**********************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<fcntl.h>
void sys_err(const char* str)
{
	perror(str);
	exit(1);	
}


int main(int argc, char* argv[])
{

	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		sys_err("fork error");
	}
	else if(pid==0)
	{
		printf("child pid=%d\n",getpid());
		printf("child gid=%d\n",getpgid(0));
		printf("child sid=%d\n",getsid(0));
		sleep(5);
		setsid();
		printf("changed\n");
		printf("child pid=%d\n",getpid());
		printf("child gid=%d\n",getpgid(0));
		printf("child sid=%d\n",getsid(0));

	}
	return 0;
}

/***********************************************************************/
/*********************************end***********************************/
/***********************************************************************/

