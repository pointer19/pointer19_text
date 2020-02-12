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
	fprintf(stderr, "%s:%s\n", str, strerror(ernu));
	exit(1);
}

void *thread_fun(void *arg)
{
	printf("child\n");
	pthread_exit(1);
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_attr_t attr;
	int pair = pthread_attr_init(&attr);
	if(pair != 0)
	{
		sys_err("attr_init error", pair);
	}
	int pasr = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(pasr != 0)
	{
		sys_err("attr setdetachstate error", pasr);
	}
	int pcr = pthread_create(&tid, &attr, thread_fun, NULL);
	if(pcr != 0)
	{
		sys_err("create error", pcr);
	}

	int padr = pthread_attr_destroy(&attr);
	if(padr != 0)
	{
		sys_err("attr destroy", padr);
	}
	sleep(1);
	return 0;
}

