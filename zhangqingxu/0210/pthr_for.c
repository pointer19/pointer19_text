#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>

void sys_err(char* str,int ernu)
{
	fprintf(stderr,"%s: %s",str,strerror(ernu));
	exit(1);
}

void* pthread_fun(void* arg)  //子线程的主函数
{
	int i=(int) arg;
	printf("i=%d,pid=%d,tid=%lu\n",i,getpid(),pthread_self());
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	int i=0;
	for(;i<5;i++)
	{
		pthread_create(&tid,NULL,pthread_fun,(void*)i);
	}
	sleep(1);
	return 0;
}

