#include<stdio.h>                                                           
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s:%s",str,strerror(ernu));
	exit(1);
}

void* pthread_crea(void* arg)
{
	while(1)
	{
		printf(".............\n");
		sleep(1);
	}
	return NULL;
}
int main(int argc,char* argv[])
{
	
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_crea,NULL);

	sleep(3);
 	pthread_cancel(tid);

	pthread_exit(NULL);
}
