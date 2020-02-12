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
	int i = (int)arg;
	printf("---%dth--- tid = %lu\n", i, pthread_self());
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int i = 0;
	for(; i < 5; i++)
	{
		int ptr = pthread_create(&tid, NULL, pthread_fun, (void*)i);
		if(ptr != 0)
		{
			sys_err("create error", ptr);
		}
	}
	printf("-------------\n");
	sleep(1);
	return 0;
}

