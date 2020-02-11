#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;
struct food
{
	struct food* next;
	int nu;
};
struct food* head = NULL;
void sys_err(char* str, int nu)
{
	fprintf(stderr, "%s:%s",str,  strerror(nu));
	exit(1);
}
void* pthread_make(void* arg)
{

	while(1)
	{
		struct food* make;
		make = (struct food*)malloc(sizeof(struct food));	 	                make->nu = rand()%500+1;
		printf("----make a food---, no = %d\n", make->nu);
		pthread_mutex_lock(&mutex);
		make->next = head;
		head = make;
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
		struct food* eat;
		pthread_mutex_lock(&mutex);
		while(head == NULL)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		eat = head;
		head = head->next;
		pthread_mutex_unlock(&mutex);
		printf("-----eat a food----------nu = %d\n", eat->nu);
		free(eat);
		sleep(rand()%5);
	}
	return NULL;
}
int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_create(&tid1, NULL, pthread_make, NULL);
	pthread_detach(tid1);
	pthread_create(&tid2, NULL, pthread_eat, NULL);
	pthread_detach(tid2);
	while(1);
	pthread_mutex_destroy(&mutex);
	printf("------------end-------------\n");
	pthread_exit(NULL);
}



