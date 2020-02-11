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
void fun1()
{
//	exit(0);结束当前进程！进程！不是线程！
	return NULL;//返回线程 线程程序没结束
}

void* pthread_fun(void *arg)
{
	printf("pid = %d tid = %lu\n",getpid(),pthread_self());
	//return NULL;
//	exit(0);
	fun1(); 
	printf("++++++++++\n");
	return NULL;//返回主线程 线程结束
}
int main(int argc, char* argv[])//主线程死的太快，所以tid无法输出 return 0结束了当前进程，内存空间被释放，线程共享的内存空间也被释放
{
	pthread_t tid;
	pthread_create(&tid,NULL,pthread_fun,NULL);
	sleep(1);
	printf("--------------");//如果是exit(0)无法打印
	return 0;//将值返回调用者 返回给bush 进程结束(主函数return 0 和 线程函数的return NULL不一样)
}

