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
{
     printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
		 return NULL;
}
int main(int agrc,char* agrv[])
{
    pid_t pid =getpid();
    pthread_t tid=pthread_self();
    pthread_t tid1;
		pthread_create(&tid1,NULL,pthread_fun,NULL);
		printf("pid=%d,tid=%lu\n",pid,tid);
		sleep(1);
		return 0;
}

