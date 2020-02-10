#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
	pid_t pid=fork();
	if(pid<0)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid==0)
	{
		printf("child pid= %d\n",getpid());
		printf("group id=%d\n",getpgid(0));
		printf("huihua id=%d\n",getsid(0));
		sleep(3);
		setsid();
		printf("changed:\n");
		printf("child pid= %d\n",getpid());
		printf("group id=%d\n",getpgid(0));
		printf("huihua id=%d\n",getsid(0));
	}
	return 0;
}

