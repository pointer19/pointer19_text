#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>

int main(int argc,char* argv[])
{
	pid_t pid;
	pid=fork();
	if(pid<0)
	{
		perror("fork");
		exit(1);
	}
	if(pid==0)
	{
		printf("child pid=%d\n",getpid());
		printf("group pid=%d\n",getpgid(0));
		printf("setsid pid=%d\n",getsid(0));
     	sleep(5);
		setsid();
		
		printf("changed\n");
		printf("child pid=%d\n",getpid());
        printf("child pid=%d\n",getpgid(0));
		printf("child pid=%d\n",getsid(0));
		while(1);
	}
	while(1);
     return 0;
}     

