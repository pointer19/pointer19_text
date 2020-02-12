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
void* pthread_fun(void *arg)
{
	int i=(int )arg;
	printf("--%dth--pid = %d tid = %lu\n",i,getpid(),pthread_self());
	return NULL;
}
int main(int argc, char* argv[])//主线程死的太快，所以tid无法输出 return 0结束了当前进程，内存空间被释放，线程共享的内存空间也被释放
{
	pthread_t tid;
	int i;//变量i存在主函数的栈区
	for(i = 0;i < 5;i++)
	{
		pthread_create(&tid,NULL,pthread_fun,(void*)i);//值传递
	}
	sleep(1);
	return 0;
}

