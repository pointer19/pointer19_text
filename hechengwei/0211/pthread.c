#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H  100000
pthread_cond_t cond;
pthread_mutex_t mutex;
int a;
struct food
{
int nu;
struct fdoo *ne;
};
struct food *head=NULL;
void * pthread_make(void *arg)
{
	while(2)
	{
	struct food *make;
	make =(struct food *)malloc(sizeof(struct food));
	make ->nu=rand()%520+1;
	printf("---make food, nu= %d----\n",make->nu);
	pthread_mutex_lock(&mutex);
	make->ne =head;
	head =make;

	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);

	sleep(rand()%3);

	}	
return NULL;

}
void *pthread_eat(void *arg)
{
	while(1)
	{
struct food *eat;
	pthread_mutex_lock(&mutex);
	if(head ==NULL)
		{
		pthread_cond_wait(&cond,&mutex);
		}

	eat =head;
head =head->ne;
	pthread_mutex_unlock(&mutex);
	printf("----------------------eat  food -nu  =%d-\n",eat->nu);
	

	free(eat);
	sleep(rand()%3);
	}
return NULL;
}
int main(int argc,char * argv[])
{
srand(time);
pthread_t tid1,tid2;
pthread_mutex_init(&mutex,NULL);
pthread_cond_init(&cond,NULL);

pthread_create(&tid1,NULL,pthread_make,NULL);
pthread_create(&tid2,NULL,pthread_eat,NULL);

pthread_join(tid1,NULL);
pthread_join(tid2,NULL);


pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond);
pthread_exit(NULL);

}
