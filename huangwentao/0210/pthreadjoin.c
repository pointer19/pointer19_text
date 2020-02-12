#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
void* pthread_crea(void* arg)
{
	printf("---- pid=%d,tid=%lu\n",getpid(),pthread_self());
	return (void*)100;	
}
int main(int argc,char* argv[])
{
pthread_t tid;
int *retval;
	pthread_create(&tid,NULL,pthread_crea,NULL);
	pthread_join(tid,(void**)&retval);
	printf("retval=%d\n",(int)retval);
pthread_exit(NULL);
}

