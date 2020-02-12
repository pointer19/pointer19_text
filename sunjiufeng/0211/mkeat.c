#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int nu;
	struct food* next;
};

struct food* head = NULL;

void sys_err(char *str, int ernu)
{
	fprintf(stderr, "%s: %s\n", str, strerror(ernu));
	exit(1);
}

void* thread_pro(void *arg)   //生产者
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		struct food* goods = (struct food*)malloc(sizeof(struct food));
		goods->nu = rand()%100+1;
		printf("+++++pro nu = %d++++\n", goods->nu);
		goods->next = head;
		head = goods;
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);

		sleep(1);
	}
	pthread_exit(NULL);
}

void* thread_cus(void *arg)   //消费者
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(head == NULL)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		struct food *f;
		f = head->next;
		printf("-----customer nu = %d-----\n", head->nu);
		head = f;
		pthread_mutex_unlock(&mutex);
		free(f);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	pthread_t tid1, tid2;
	int pmir = pthread_mutex_init(&mutex, NULL);
	if(pmir != 0)
	{
		sys_err("mutex init error", pmir);
	}
	int pcir = pthread_cond_init(&cond, NULL);
	if(pcir != 0)
	{
		sys_err("cond init error", pcir);
	}

	int pcr1 = pthread_create(&tid1, NULL, thread_pro, NULL);
	if(pcr1 != 0)
	{
		sys_err("create1 error", pcr1);
	}
	int pcr2 = pthread_create(&tid2, NULL, thread_cus, NULL);
	if(pcr2 != 0)
	{
		sys_err("create2 error", pcr2);
	}
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	int pmdr = pthread_mutex_destroy(&mutex);
	if(pmdr != 0)
	{
		sys_err("mutex destroy error", pmdr);
	}
	int pcdr = pthread_cond_destroy(&cond);
	if(pcdr != 0)
	{
		sys_err("cond destroy error", pcdr);
	}

	pthread_exit(NULL);
}

