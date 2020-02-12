#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H 1000000
pthread_mutex_t   mutex;   //创建了一把锁
int a=0;
void *pthread_fun(void*arg)
{
	int i;
	for(i=0;i<H;i++)
	{
		 pthread_mutex_lock(&mutex) ;
		a++;
		printf("a=%d\n",a);
		 pthread_mutex_unlock(&mutex) ;
	}
	return NULL;
}
int main()
{
	pthread_t tid;
	 pthread_mutex_init(&mutex ,NULL); 
	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_detach(tid);
	int i;
	for(i=0;i<H;i++)
	{
		 pthread_mutex_lock(&mutex) ;
		a++;
		printf("---a=%d---\n",a);
		 pthread_mutex_unlock(&mutex) ;
	}
	pthread_mutex_destroy(&mutex)  ;
	pthread_exit(NULL);
}


