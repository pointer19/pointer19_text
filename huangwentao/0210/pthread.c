#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>//
int main(int argc,char* argv[])
{
	pid_t pid=getpid();
	pthread_t tid=pthread_self();
	printf("pid=%d,tid=%lu\n",pid,tid);

return 0;
}

