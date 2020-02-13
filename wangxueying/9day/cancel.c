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
	while(1)
	{
//		printf("------\n");
//		sleep(1);
		pthread_testcancel();
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_create(&tid, NULL, pth_fun, NULL);

	sleep(3);
	pthread_cancel(tid);

	pthread_exit(NULL);
}

