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
	printf("pid =%d, tid =%lu\n",getpid(),pthread_self());
	return (void *)100;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int *retval;
	pthread_create(&tid, NULL, pth_fun, NULL);

	pthread_join(tid,(void**)&retval);

	printf("retval= %d\n",(int )retval);
	sleep(2);
	return 0;
}

