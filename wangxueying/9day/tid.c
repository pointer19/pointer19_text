#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>

void sys_err(char *str)
{
	perror(str);
	exit(1);
}

int main(int argc, char *argv[])
{
	pid_t pid=getpid();
	pthread_t tid =pthread_self();

	printf("pid = %d ,tid = %lu\n",pid,tid);
	return 0;
}

