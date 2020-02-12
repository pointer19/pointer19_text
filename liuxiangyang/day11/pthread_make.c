#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int nu;
	struct food*next;	
};
struct food*head=NULL;
void* pthread_make(void* arg)
{
	while(1)
	{
		struct food* make;
		make=(struct food*)malloc(sizeof(struct food));
		make->nu=rand()%500+1;
		printf("***make nu=%d\n",make->nu);
		pthread_mutex_lock(&mutex);
		make->next=head;
		head=make;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);

		sleep(rand()%5);
	}
	return NULL;

}
void* pthread_eat(void* arg)
{
	while(1)
	{
		struct food*eat;
		pthread_mutex_lock(&mutex);
		if(head==NULL)
		{
			pthread_cond_wait(&cond,&mutex);
		}
		eat=head;
		head=head->next;
		pthread_mutex_unlock(&mutex);
		printf("------eat nu=%d\n",eat->nu);
		free(eat);
		sleep(rand()%5);
	}
	return NULL;
}  
int main(int argc,char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	
	pthread_create(&tid1,NULL,pthread_make,NULL);
	pthread_create(&tid2,NULL,pthread_eat,NULL);
	 


	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond); 
     return 0;
}     

