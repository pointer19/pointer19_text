#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
int num;
struct food* next;	
};
struct food* head=NULL;
void* make(void* num)
{
	while(1)
	{
		struct food* mak;
		mak=(struct food*)malloc(sizeof(struct food));
		mak->num=rand()%10+1;
		
		pthread_mutex_lock(&mutex);
		mak->next=head;
		head=mak;
		pthread_mutex_unlock(&mutex);
			
		pthread_cond_signal(&cond);
		printf("----make-----\n");
		sleep((rand()%4+1));
	}
}
void* eat(void* num)
{
	while(1)
	{
		struct food* eat;
		pthread_mutex_lock(&mutex);
		while(head==NULL)
		{
			pthread_cond_wait(&cond,&mutex);
				
		}
		eat=head;
		head=head->next;
		pthread_mutex_unlock(&mutex);
		free(eat);
		printf("----eat----\n");
		sleep((rand()%4+1));


	}
}
int main(int argc,char* argv[])
{
	srand(time(0));
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);

pthread_t tid1,tid2;

pthread_create(&tid1,NULL,make,NULL);
pthread_create(&tid2,NULL,eat,NULL);

pthread_join(tid1,NULL);
pthread_join(tid2,NULL);

pthread_mutex_destroy(&mutex);
printf("main");
pthread_exit(NULL);
}

