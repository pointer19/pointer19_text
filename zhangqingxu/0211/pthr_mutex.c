#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
	int a=0;
	pthread_mutex_t mutex;

void sys_err(char* str,int erno)
{
	fprintf(stderr,"%s:%s\n",str,strerror(erno));
	exit(1);
}

void* pthread_fun(void* arg)
{
	for(int i=0;i<1000;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("----%d----\n",a);
		pthread_mutex_unlock(&mutex);
	}
	return (void*)100;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex,NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	pthread_create(&tid,&attr,pthread_fun,NULL);
	for(int i=0;i<1000;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("----%d----\n",a);
		pthread_mutex_unlock(&mutex);
	}
	sleep(5);
	pthread_mutex_destroy(&mutex);
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

