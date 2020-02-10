#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork error");
		exit(1);
	}
	if(pid == 0)
	{
		printf("child pid = %d\n",getpid());
		printf("child gid = %d\n",getpgid(0));
		printf("child sid = %d\n",getsid(0));
		
		sleep(10);
		setsid();
		printf("changed:\n");

		printf("child pid = %d\n",getpid());
		printf("child gid = %d\n",getpgid(0));
		printf("child sid = %d\n",getsid(0));
		sleep(20);
	}

	return 0;
}
