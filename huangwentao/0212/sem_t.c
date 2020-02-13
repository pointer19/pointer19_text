#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
sem_t p;
sem_t t;
int a[5]={0};
void* pro(void* argc)
{
	int i=0;
	while(1)
	{
		sem_wait(&p);
		a[i]=rand()%10;
		printf("--make--=ai=%d\n",a[i]);
		sem_post(&t);
		i=(i+1)%5;
		sleep(rand()%3);	
	}
}
void* cus(void* argc)
{	
	int i=0;
	while(1)
	{
		sem_wait(&t);
		
		printf("++read++=ai=%d\n",a[i]);
		a[i]=0;
		sem_post(&p);
		i=(i+1)%5;
		sleep(rand()%3);	
	}
}
int main(int argc,char* argv[])
{
	srand(0);
	pthread_t tid1,tid2;
	sem_init(&p,0,5);
	sem_init(&t,0,0);
	pthread_create(&tid1,NULL,pro,NULL);
	pthread_create(&tid2,NULL,cus,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
	sem_destroy(&p);
	sem_destroy(&t);
	pthread_exit(NULL);
}

