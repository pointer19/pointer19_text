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
	printf("tid=%lu\n",pthread_self());
	while(1)
	{
//		printf("*********\n");
//		sleep(1);
		pthread_testcancel();
	}
}

int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	sleep(5);
	pthread_cancel(tid);
	return 0;
}

