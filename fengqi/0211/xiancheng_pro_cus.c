#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H 500
struct food
{
	 int num;
	 struct food* next;
};

struct food* head=NULL;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* pthread_productor(void* arg)
{
		int i=0;
		while(1)
		{
				struct food* productor;
				productor=(struct food*)malloc(sizeof(struct food));
				productor->num=i++;
				printf("product a food : num=%d",productor->num);
        pthread_mutex_lock(&mutex);
				productor->next=head;
				head=productor;
				pthread_mutex_unlock(&mutex);
				pthread_cond_signal(&cond);
				sleep(rand()%5);
		}
		return NULL;
}
void* pthread_customer(void* arg)
{
		while(1)
		{
				struct food* customer;
        pthread_mutex_lock(&mutex);
				if(head==NULL)
				{
						pthread_cond_wait(&cond,&mutex);
				}
				customer=head;
				head=head->next;
				pthread_mutex_unlock(&mutex);
				printf("consume a food ---num=%d\n",customer->num);
				free(customer);
				sleep(rand()%5);
		}
		return NULL;
}
void sys_err(char* str,int num)
{
		fprintf(stderr,"%s:%s\n",str,strerror(num));
		pthread_exit(NULL);
}
int main(int agrc,char* agrv[])
{
		srand(time(NULL));
    pthread_t tid[2];
		pthread_mutex_init(&mutex,NULL);
		pthread_cond_init(&cond,NULL);
    pthread_create(&tid[0],NULL,pthread_productor,NULL);
		pthread_detach(tid[0]);
	  pthread_create(&tid[1],NULL,pthread_customer,NULL);
		pthread_detach(tid[1]);
		while(1);
    pthread_mutex_destroy(&mutex);
		pthread_exit(NULL);
}

