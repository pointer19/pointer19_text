#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>
#include<semaphore.h>

#define N 10
sem_t pro;
sem_t con;
pthread_cond_t cond;
int arry[N];

void *Producer(void *a)
{
	int i=0;
	while(1)
	{
		sem_wait(&pro);
		arry[i]=rand()%100+1;
		sem_post(&con);

		printf("write:%d\n",arry[i]);
		i=(i+1)%N;
	}
}

void *Consumer(void *a)
{
	int i=0;
	while(1)
	{
		printf("read:%d\n",arry[i]);
		sem_wait(&con);
		arry[i]=0;
		sem_post(&pro);
		i=(i+1)%10;
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	sem_init(&pro, 0, N);
	sem_init(&con, 0, 0);
	pthread_create(&tid1, NULL, Producer, NULL);
	pthread_create(&tid2, NULL, Consumer, NULL);

	pthread_detach(tid1);
	pthread_detach(tid2);
	while(1);
	sem_destroy(&pro);
	sem_destroy(&con);
	printf("end\n");
	pthread_exit(NULL);

}

