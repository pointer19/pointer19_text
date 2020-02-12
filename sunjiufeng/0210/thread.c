#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

void sys_err(int ernu)
{
	
}

int main(int argc, char* argv[])
{
	pid_t pid = getpid();
	pthread_t tid = pthread_self();

	printf("pid = %d, tid = %lu\n", pid, tid);
	return 0;
}

