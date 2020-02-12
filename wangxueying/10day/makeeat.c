#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int n;
	struct food *next;
};

struct food *head=NULL;

void * pth_make(void *arg)
{
	while(1)
	{
		struct food *make;
		make=(struct food *)malloc(sizeof(struct food));
		make->n=rand()%500+1;

		printf("+++++make+++%d\n",make->n);

		pthread_mutex_lock(&mutex);
		make->next=head;
		head=make;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&cond);

		sleep(rand()%3);
	}
	return NULL;
}

void * pth_eat(void *arg)
{
	while(1)
	{
		struct food* eat;
		pthread_mutex_lock(&mutex);
		if(head==NULL)
		{
			pthread_cond_wait(&cond,&mutex);
		}

		eat=head;
		head=head->next;
		pthread_mutex_unlock(&mutex);

		printf("------------eat---%d\n",eat->n);

		free(eat);
		sleep(rand()%5);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	
	pthread_create(&tid1,NULL,pth_make,NULL);
	pthread_create(&tid2,NULL,pth_eat,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
	
	pthread_exit(NULL);
}

