#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
void sys_error(char *str,int ernu	)//打印错误信息
{
	fprintf(stderr,"%s:,%s:",str,strerror(ernu));
	exit(1);
}
int a=20;//data段，共享
void* pthread_fun(void *arg)    //子线程
{
	a=50;
	printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
	return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	sleep(1);
	printf("a=%d\n",a);
	return 0;
}
