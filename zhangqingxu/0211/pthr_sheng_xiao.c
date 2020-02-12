#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct goods
{
	int no;
	struct goods* next;
};

struct goods* head=NULL; 

void* pthread_make(void* arg)
{
	int i=0;
	while(1)
	{
		struct goods* make;
		make =(struct goods*)malloc(sizeof(struct goods));
		make->no=i++;

		printf("make a goods :---%d---\n",make->no);
		pthread_mutex_lock(&mutex);
		make->next=head;
		head=make;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
		sleep(rand()%5);
	}
	return NULL;
}

void* pthread_sell(void* arg)
{
	while(1)
	{
		struct goods* shop;
		pthread_mutex_lock(&mutex);
		if(head==NULL)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		shop=head;
		head=head->next;
		pthread_mutex_unlock(&mutex);
		printf("shop a goods:---%d---\n",shop->no);
		free(shop);
		sleep(rand()%3);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	pthread_t tid1,tid2;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	pthread_create(&tid1,&attr,pthread_make,NULL);	
	pthread_create(&tid2,&attr,pthread_sell,NULL);	
	while(1);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

