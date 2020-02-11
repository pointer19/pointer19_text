#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<pthread.h>
void sys_errr(char *str,int ernu)
{
	fprintf(stderr,"%s: %s",str,strerror(ernu));
	exit(1);
}
void* fun1()
{
	pthread_exit(NULL);
}

void* pthread_fun(void *arg)
{
	sleep(2);
	printf("pid = %d tid = %lu\n",getpid(),pthread_self());
	pthread_exit(NULL);
}
int main(int argc, char* argv[])//主线程死的太快，所以tid无法输出 return 0结束了当前进程，内存空间被释放，线程共享的内存空间也被释放
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_exit(NULL);
}

