#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#include<semaphore.h>

#define H 10
sem_t space;
sem_t full;
int a[10]={0};
void* pthread_productor(void* arg)
{
		int i=0;
		while(1)
		{
        sem_wait(&space);
				a[i]=rand()%500+1;
        printf("product----%d------\n",a[i]);
				sem_post(&full);
				i=(i+1)%H;
				sleep(rand()%3);
		}
		return NULL;
}
void* pthread_customer(void* arg)
{
		int i=0;
		while(1)
		{
				sem_wait(&full);
        printf("customer----%d------\n",a[i]);
				a[i]=0;
				sem_post(&space);
				i=(i+1)%H;
				sleep(rand()%3);
		}
		return NULL;
}
void sys_err(char* str,int num)
{
		fprintf(stderr,"%s:%s\n",str,strerror(num));
		pthread_exit(NULL);
}
int main(int agrc,char* agrv[])
{
		srand(time(NULL));
    pthread_t tid1,tid2;
		sem_init(&full,0,0);
    sem_init(&space,0,H);
    int ret1= pthread_create(&tid1,NULL,pthread_productor,NULL);
		if(ret1!=0)
		{
       sys_err("pthread_productor error",ret1);
		}
	  int ret2=pthread_create(&tid2,NULL,pthread_customer,NULL);
    if(ret2!=0)
		{
        sys_err("pthread_customer error",ret2);
		}
		int ret;
		ret=pthread_join(tid1,NULL);
		if(ret!=0)
		{
         sys_err("tid1 error",ret);
		}
		ret=pthread_join(tid2,NULL);
		if(ret!=0)
		{
         sys_err("tid2 error",ret);
		}
    sem_destroy(&space);
		sem_destroy(&full);
		pthread_exit(NULL);
}

