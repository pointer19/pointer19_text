#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#define H 1000
pthread_mutex_t mutex;
int a=0;

void *pth_new(void *arg)
{
	int i;
	for(i=0;i<H;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("+++++++++++a++%d\n",a);
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex,NULL);

	pthread_create(&tid,NULL,pth_new,NULL);
	pthread_detach(tid);
	
	int i;
	for(i=0;i<H;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("+++a++%d\n",a);
		pthread_mutex_unlock(&mutex);
	}
	while(1);
	pthread_mutex_destroy(&mutex);

	pthread_exit(NULL);
}

