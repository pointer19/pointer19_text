#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
	int a=0;
	pthread_rwlock_t rwlock;

void sys_err(char* str,int erno)
{
	fprintf(stderr,"%s:%s\n",str,strerror(erno));
	exit(1);
}

void* pthread_rfun(void* arg)
{
	for(int j=0;j<10;j++)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("read-%dth :-----%d-----\n",(int)arg,j);
		pthread_rwlock_unlock(&rwlock);
		usleep(1000);
	}
	return (void*)100;
}

void* pthread_wfun(void* arg)
{
	for(int j=0;j<10;j++)
	{
		pthread_rwlock_wrlock(&rwlock);
		printf("write-%dth:-----%d-----\n",(int)arg,j);
		pthread_rwlock_unlock(&rwlock);
		usleep(1000);
	}
	return (void*)100;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_rwlock_init(&rwlock,NULL);
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	int i=0;
	for(i=0;i<5;i++)
	{
		pthread_create(&tid,&attr,pthread_rfun,(void*)i);
	}
	for(i=5;i<10;i++)
	{
		pthread_create(&tid,&attr,pthread_wfun,(void*)i);
	}
	while(1);
	pthread_rwlock_destroy(&rwlock);
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

