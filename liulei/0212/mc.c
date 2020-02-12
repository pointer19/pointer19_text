#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int nu;
	struct food *next;
};

struct food *head=NULL;
void *Producer(void *a)
{
	struct food *make;
	while(1)
	{
		make=(struct food*)malloc(sizeof(struct food));
		make->nu=rand()%100+1;

		pthread_mutex_lock(&mutex);
		make->next=head;
		head=make;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&cond);
		printf("write:%d\n",head->nu);
		sleep(1);
	}
}

void *Consumer(void *a)
{
	struct food *eat;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(head == NULL)
			pthread_cond_wait(&cond, &mutex);

		eat=head;
		head=head->next;
		pthread_mutex_unlock(&mutex);

		printf("read:%d\n",eat->nu);
		free(eat);
		sleep(1);
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid1, NULL, Producer, NULL);
	pthread_create(&tid2, NULL, Consumer, NULL);

	pthread_detach(tid1);
	pthread_detach(tid2);
	while(1);
	pthread_mutex_destroy(&mutex);
	printf("end\n");
	pthread_exit(NULL);

}

