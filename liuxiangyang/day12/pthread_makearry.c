#include<stdio.h>                                                           
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include<semaphore.h>
void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s:%s",str,strerror(ernu));
	exit(1);
}
#define N 10
int arry[N]={0} ;
sem_t sem_space;
sem_t sem_full;

void* pthread_prodocer(void* arg)
{
	int i;
	while(1)
	{
		sem_wait(&sem_space);
		arry[i]=rand()%500+1;
		printf("+++producer nu==%d\n",arry[i]);
		sem_post(&sem_full);

	i=(i+1)%N;


		sleep(rand()%3);
	}
	return NULL;
}

void* pthread_cunsumer(void* arg)
{
	int i=0;
	while(1)
	
	{
		sem_wait(&sem_full);
		printf("--------------csumer nu=%d\n",arry[i]);
		arry[i]=0;
		sem_post(&sem_space);

		i=(i+1)%N;

		sleep(rand()%3);
	
	}

    return NULL;
}

int main(int argc,char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	int ret;
	
	sem_init(&sem_space,0,10);
	sem_init(&sem_full,0,0);

    ret=pthread_create(&tid1,NULL,pthread_prodocer,NULL);
	if(ret!=0)
	{
		sys_err("tid1 creat error",ret);
	}
	ret=pthread_create(&tid2,NULL,pthread_cunsumer,NULL);


	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);   
	
	sem_destroy(&sem_space);
	sem_destroy(&sem_full);
	
	pthread_exit(NULL);

}
