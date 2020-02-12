#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

void sys_err(char *str, int ernu)
{
	fprintf(stderr, "%s: %s\n", str, strerror(ernu));
	exit(1);
}

void *pthread_fun(void *arg)
{
	printf("++++++++++++\n");
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int ptr = pthread_create(&tid, NULL, pthread_fun, NULL);
	if(ptr != 0)
	{
		sys_err("create error", ptr);
	}
	printf("-------------\n");
	sleep(1);
	return 0;
}

