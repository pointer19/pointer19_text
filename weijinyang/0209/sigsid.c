#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main(int argc, char* argv[])
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
	perror("fork error");
	exit(1);
    }
    else if(pid == 0)
    {
	printf("child pid is %d\n", getpid());
	printf("group id is %d\n", getpgid(0));
	printf("session id is %d\n", getsid(0));
	sleep(10);
	setsid();
	printf("change:\n");
	printf("child pid is %d\n", getpid());
	printf("group id is %d\n", getpgid(0));
	printf("session id is %d\n", getsid(0));
	while(1);
    }
    else
    {
	exit(1);
    }
    return 0;
}

