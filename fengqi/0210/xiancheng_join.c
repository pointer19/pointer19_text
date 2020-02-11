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
		 return (void*)100;
}
int main(int agrc,char* agrv[])
{
		int * retval;
    pthread_t tid1;
		pthread_create(&tid1,NULL,pthread_fun,NULL);
		pthread_join(tid1,(void**)&retval);
		printf("retval=%d\n",(int)retval);//zusehuishou
		pthread_exit(NULL);
}

