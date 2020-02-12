#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#include<semaphore.h>

void err(char *str,int num)
{
	fprintf(stderr,"%s,%s\n",str,strerror(num));
	exit(1);	
}
#define N 10

int arry[N]={0};

sem_t sem_space;
sem_t sem_full;

void* pthread_producter(void* arg)
{
	int i=0;
	while(1)
		{
			sem_wait(&sem_space);//看有无空格
			arry[i]=rand()%500+1;
			printf("++producter no = %d\n",arry[i]);
			sem_post(&sem_full);//加1
			
			i=(i+1)%N;
			sleep(rand()%3);	
		}
		return NULL;
}
void* pthread_consumer(void* arg)
{
	int i=0;
	while(1)
		{
			sem_wait(&sem_full);
			arry[i]=rand()%500+1;
			printf("--------------consumer no = %d\n",arry[i]);
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

	sem_init(&sem_space,0,N);
	sem_init(&sem_full,0,0);

	ret=pthread_create(&tid1,NULL,pthread_producter,NULL);
	if(ret!=0)
	{
		err("pthread_create error",ret);	
	}
	
	ret=pthread_create(&tid2,NULL,pthread_consumer,NULL);
	if(ret!=0)
	{
		err("pthread_create error",ret);	
	}

	ret=pthread_join(tid1,NULL);
	if(ret!=0)
	{
		err("pthread_join error",ret);	
	}
	
	ret=pthread_join(tid2,NULL);
	if(ret!=0)
	{
		err("pthread_join error",ret);	
	}

	sem_destroy(&sem_space);
	sem_destroy(&sem_full);
	pthread_exit(NULL);
	
	return 0;	
}

