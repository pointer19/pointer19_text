#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#define H 1000
int a=0;
//协调线程工作顺序、防止多线程同时访问共享区域、防止数据混乱
//锁的是线程，建议锁，为了防止访问公共数据时出现数据混乱
//访问公共数据前先拿锁

//使用mutex互斥量的一般步骤
pthread_mutex_t mutex;//定义锁，全局变量
void* pthread_fun(void *arg)
{
	int i;
	for(i=0;i<H;i++)
	{
		pthread_mutex_lock(&mutex);
		a++;
		printf("a=%d\n",a);
		pthread_mutex_unlock(&mutex);
	}
}
int main(int argc,char* argv[])
{
	pthread_t tid;
	pthread_mutex_init(&mutex,NULL);//初始化锁，传锁指针

	pthread_create(&tid,NULL,pthread_fun,NULL);
	pthread_detach(tid);

	int i;
	for(i=0;i<H;i++)
	{
		pthread_mutex_lock(&mutex);//加锁  之后访问公共数据
		a++;
		printf("------a=%d------\n",a);
		pthread_mutex_unlock(&mutex);//解锁
	}
	while(1);
	pthread_mutex_destroy(&mutex);//销毁锁
	pthread_exit(NULL);
}

