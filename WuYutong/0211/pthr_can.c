#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
void sys_error(char *str,int ernu)
{
	fprintf(stderr,"%s:,%s:",str,strerror(ernu));
	exit(1);
}
void* pthread_fun(void *arg)
{
	printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
	while(1)
	{
		pthread_testcancel();
	}
	return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	sleep(3);
	pthread_cancel(tid);
	pthread_exit(NULL);
}

