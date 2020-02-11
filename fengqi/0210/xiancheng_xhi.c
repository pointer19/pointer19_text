#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void sys_err(char* str,int erno)
{
		printf("%s:%s",str,strerror(erno));
}

void* pthread_fun(void *arg)
{    int i= (int)arg;
     printf("i=%dst,pid=%d,tid=%lu\n",i,getpid(),pthread_self());
		 return NULL;
}
int main(int agrc,char* agrv[])
{
    pid_t pid =getpid();
    pthread_t tid=pthread_self();
    pthread_t tid1;
		for(int i=0;i<5;i++)
		{
			pthread_create(&tid1,NULL,pthread_fun,(void*)i);
		}
		printf("pid=%d,tid=%lu\n",pid,tid);
		sleep(1);
		return 0;
}

