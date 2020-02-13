#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#define H 1000
int a=0;

pthread_rwlock_t rwlock;

void* pthread_wfun(void * arg)
{
	while(1)
	{
			pthread_rwlock_wrlock(&rwlock);
			int b=a;
			a++;
			printf("-write-%dth---olda=%d,newa=%d\n",(int)arg,b,a);
			pthread_rwlock_unlock(&rwlock);
			usleep(4000);
	}
	
}
void* pthread_rfun(void * arg)
{
	while(1)
	{
			pthread_rwlock_rdlock(&rwlock);
			printf("----read-%dth-----------a=%d-\n",(int)arg,a);
			pthread_rwlock_unlock(&rwlock);
			usleep(3000);
	}
	return NULL;
}
int main(int argc,char* argv[])
{
	pthread_t tid[10];
	pthread_rwlock_init(&rwlock,NULL);
	
	for(int i=0;i<4;i++)
	{
		pthread_create(&tid[i],NULL,pthread_wfun,(void*)i);
		pthread_detach(tid);
	}
	for(int i=0;i<10;i++)
	{
		pthread_create(&tid[i],NULL,pthread_rfun,(void*)i);
		pthread_detach(tid);
	}
	pthread_rwlock_destroy(&rwlock);
	pthread_exit(NULL);
}

