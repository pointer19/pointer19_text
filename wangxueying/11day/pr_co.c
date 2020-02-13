#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#include<signal.h>
#include<semaphore.h>
#define H 10

sem_t full;//已占用空间
sem_t need;//剩余空间

int a[H]={0};

void sys_err(char *str, int signum)
{
	fprintf(stderr,"%s:%s\n",str,strerror(signum));
	exit(1);
}

void* pth_pro(void *arg)
{
	int i=0;
	while(1)
	{
		
		sem_wait(&need);
		a[i]=rand()%500+1;
		printf("+++%d+++\n",a[i]);
		sem_post(&full);
		i=(i+1)%H;
		sleep(rand()%3);
	}
	return NULL;
}
void* pth_cus(void *arg)
{
	int i=0;
	while(1)
	{
		sem_wait(&full);
		printf("------------%d--------\n",a[i]);
		a[i]=0;
		sem_post(&need);
		i=(i+1)%H;
		sleep(rand()%3);
	}
	return NULL;
}
int main(int argc, char *argv[])
{
	pthread_t tid1,tid2;
	srand(time(NULL));
	int ret;
	sem_init(&full,0,0);
	sem_init(&need,0,H);

	ret = pthread_create(&tid1,NULL,pth_pro,NULL);
	if(ret != 0)
	{
		sys_err("create tid1 error",ret);
	}

	ret = pthread_create(&tid2,NULL,pth_cus,NULL);
	if(ret != 0)
	{
		sys_err("create tid2 error",ret);
	}

	ret = pthread_join(tid1,NULL);
	if(ret != 0)
	{
		sys_err("join tid1 error",ret);
	}
	ret = pthread_join(tid2,NULL);
	if(ret != 0)
	{
		sys_err("join tid2 error",ret);
	}
	
	sem_destroy(&full);
	sem_destroy(&need);
	pthread_exit(NULL);
}

