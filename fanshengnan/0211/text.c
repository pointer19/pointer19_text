#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

#define H 10000
int a=0;
void *pthread_fun(void*arg)
{
	int i;
	for(i=0;i<H;i++)
	{
		a++;
		printf("a=%d\n",a);
	}
	return NULL;
}
int main()
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_detach(tid);
	int i;
	for(i=0;i<H;i++)
	{
		a++;
		printf("---a=%d---\n",a);
	}
	pthread_exit(NULL);
}


