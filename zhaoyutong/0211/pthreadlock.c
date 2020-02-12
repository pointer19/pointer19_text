#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#define H 10000
int a = 0;
pthread_mutex_t mutex;
void sys_err(char* str, int nu)
{
	fprintf(stderr,"%s:%s",str,strerror(nu));
	exit(1);
}
void* pthread_fun(void* arg)
{
	int i;
	for(i = 0; i < H; i++)
	{
	    pthread_mutex_lock(&mutex);
	    a++;
            printf("a = %d\n", a);
	    pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_detach(tid);
	int i;
	for(i = 0; i < H; i++)
	{
	    pthread_mutex_lock(&mutex);
	    a++;
	    printf("----a = %d-----\n", a);
	    pthread_mutex_unlock(&mutex);
	}
        while(1);
	pthread_mutex_destroy(&mutex);
        return 0;
  }


