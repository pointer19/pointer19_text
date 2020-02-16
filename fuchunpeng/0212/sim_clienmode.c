/***************************************************************************************************************************************************************************************************************************************************************Mr.Fu**********************************************************************************************/





#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>		    
#include<pthread.h>
#include<semaphore.h>

#define N 10
int sth[N];
sem_t space;
sem_t thing;
int i=0;
int j=0;

void sys_err(char *str,int erno)
{
	fprintf(stderr,"%s: %s\n",str,strerror(erno));
	exit(1);
}

void* pthread_producer(char* arg)
{
	while(1)
	{
		sem_wait(&space);
		sth[i]=rand()%500+1;
		printf("-make thing is sth[%d]=%d\n",i,sth[i]);
		i=(i+1)%5;
		sem_post(&thing);
		sleep(rand()%3);
	}
	return NULL;
}

void* pthread_client(char *arg)
{
	while(1)
	{
		sem_wait(&thing);
		int k=sth[j];
		printf("-get thing is sth[%d]=%d\n",j,k);
		j=(j+1)%5;

		sem_post(&space);
		sleep(rand()%3);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	sem_init(&space,0,N);
	sem_init(&thing,0,0);
	pthread_create(&tid1,NULL,pthread_producer,NULL);
	pthread_create(&tid2,NULL,pthread_client,NULL);
	pthread_join(&tid1,NULL);
	pthread_join(&tid2,NULL);
	sem_destroy(&space);
	sem_destroy(&thing);
	pthread_exit(NULL);
}





/***************************************************************************************************************************************************************************************************************************************************************Mr.Fu**********************************************************************************************/

