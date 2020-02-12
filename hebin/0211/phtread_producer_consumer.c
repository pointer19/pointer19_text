#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int nu;
	struct food* next;
};
struct food *head=NULL;
void* pthread_producer(void *arg)
{
	while(1)
	{
		struct food *produce;
		produce = (struct food*)malloc(sizeof(struct food));
		produce->nu=rand()%100+1;
		printf("--------produce a food %d----------\n",produce->nu);
		pthread_mutex_lock(&mutex);
		produce->next = head;
		head = produce;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		sleep(rand()%5);
	}
}

void* pthread_consumer(void *arg)
{
	while(1)
	{
		struct food* consume;
		pthread_mutex_lock(&mutex);
		if(head==NULL)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		consume = head;
		head = head->next;
		pthread_mutex_unlock(&mutex);
		printf("$$$$$$$$$$$consume a food %d$$$$$$$$$$$$$\n",consume->nu);
		free(consume);
		sleep(rand()%5);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_create(&tid1,NULL,pthread_producer,NULL);
	pthread_create(&tid2,NULL,pthread_consumer,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	return 0;
}

