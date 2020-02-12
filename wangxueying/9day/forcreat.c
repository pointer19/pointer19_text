#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>

void sys_err(char *str, int ernu)
{
	fprintf(stderr,"%s: %s",str,strerror(ernu));
	exit(1);
}
void* pth_fun(void * arg)
{
	int i=(int)arg;
	printf("---%d----pid =%d, tid =%lu\n",i,getpid(),pthread_self());
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;

	int i;
	for(i=0; i<5; i++)
	{
		pthread_create(&tid, NULL, pth_fun, (void*)i);
	}

	sleep(2);
	return 0;
}

