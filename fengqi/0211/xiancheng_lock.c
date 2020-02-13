#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H 500

int a=0;
pthread_mutex_t mutex;


void* pthread_fun(void* arg)
{
		int i=0;
		for(i=0;i<H;i++)
		{
        pthread_mutex_lock(&mutex);
				a++;
				printf("a=%d\n",a);
				pthread_mutex_unlock(&mutex);
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
    pthread_t tid;
		pthread_mutex_init(&mutex,NULL);
    pthread_create(&tid,NULL,pthread_fun,NULL);
		pthread_detach(tid);
		int i=0;
		for(i=0;i<H;i++)
		{
       pthread_mutex_lock(&mutex);
			 a++;
			 printf("a=%d\n",a);
			 pthread_mutex_unlock(&mutex);
		}
    pthread_mutex_destroy(&mutex);
		return 0;
}

