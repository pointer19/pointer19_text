#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem_p,sem_c;
int arry[5]={0};
void* pthread_consumer(void* arg)
{
	int i=0;
	while(1)
	{
		sem_wait(&sem_c);
		printf("buy a goods : ---%d\n",arry[i]);
		arry[i]=0;
		sem_post(&sem_p);
		i=(i+1)%5;
		sleep(rand()%3);
	}
	return NULL;
}

void* pthread_produce(void* arg)
{
	int i=0;
	while(1)
	{
		sem_wait(&sem_p);
		arry[i]=rand()%10+1;
		printf("make a goods:%d\n",arry[i]);
		sem_post(&sem_c);
		i=(i+1)%5;
		sleep(rand()%3);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	sem_init(&sem_p,0,5);
	sem_init(&sem_c,0,0);
	pthread_attr_t attr;
	pthread_attr_init(&attr);	
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	pthread_create(&tid1,&attr,pthread_produce,NULL);
	pthread_create(&tid1,&attr,pthread_consumer,NULL);

	while(1);
	sem_destroy(&sem_p);
	sem_destroy(&sem_c);
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

