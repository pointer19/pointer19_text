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
pthread_mutex_t mutex;

void sys_err(char *str, int ernu)
{
	fprintf(stderr, "%s:%s\n", str, strerror(ernu));
}

void* pthread_fun(void *arg)
{
	int i = 0;
	for(; i < H; i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("tid = %lu -> a = %d\n", pthread_self(), a);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);
	int ptr = pthread_create(&tid, NULL, pthread_fun, NULL);
	if(ptr != 0)
	{
		sys_err("create error", ptr);
	}

	for(int i = 0; i < H; i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("tid = %lu -> a = %d\n", pthread_self(), a);
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
}

