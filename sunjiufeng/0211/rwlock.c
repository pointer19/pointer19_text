#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>

#define H 10000
int a = 0;
pthread_rwlock_t rwlock;

void* thread_rfun(void *arg)
{
	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("+++++read %dth, a = %d+++++\n", (int)arg);
		pthread_rwlock_unlock(&rwlock);
		usleep(3000);
	}
	pthread_exit(NULL);
}

void* thread_wfun(void *arg)
{
	while(1)
	{
		pthread_rwlock_wrlock(&rwlock);
		int b = a;
		a++;
		printf("------write %dth-----\n", (int)arg);
		pthread_rwlock_unlock(&rwlock);
		usleep(4000);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t tid[10];
	pthread_rwlock_init(&rwlock, NULL);
	int i ;
	for(i = 0; i < 4; i++)
	{
		pthread_create(&tid[i], NULL, thread_wfun, (void*)i);
		pthread_detach(tid);
	}
	for(i = 4; i < 10; i++)
	{
		pthread_create(&tid[i], NULL, thread_rfun, (void*)i);
		pthread_detach(tid);	
	}
	pthread_rwlock_destroy(&rwlock);
	pthread_exit(NULL);
}
