#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#define N 10//宏定义
void sys_err(char* str,int nu)//错误函数
{
	fprintf(stderr,"%s:%s\n",str,strerror(nu));
	exit(1);
}
int arry[N]={0};//公共区域
sem_t sem_space;//信号量 剩余空间
sem_t sem_full;//信号量 占用空间
void* pthread_producer(void* arg)//生产者线程
{
	int i=0;
	while(1)
	{
		sem_wait(&sem_space);//剩余空间--
		arry[i]=rand()%500+1;
		printf("+++++producer nu=%d\n",arry[i]);
		sem_post(&sem_full);//占用空间++
		i=(i+1)%N;
		sleep(rand()%3);
	}
	pthread_exit(NULL);
}
void* pthread_consumer(void* arg)//消费者线程
{
	int i=0;
	while(1)
	{
		sem_wait(&sem_full);//占用空间--
		printf("-----consumer nu=%d\n",arry[i]);
		arry[i]=0;
		sem_post(&sem_space);//剩余空间++
		i=(i+1)%N;
		sleep(rand()%3);
	}
	pthread_exit(NULL);
}
int main(int argc,char* argv[])
{
	srand(time(NULL));//随机数种子
	pthread_t tid1,tid2;//线程id
	sem_init(&sem_space,0,N);//剩余空间信号量初始化
	sem_init(&sem_full,0,0);//占用空间信号量初始化
	int ret;//记录错误值
	ret=pthread_create(&tid1,NULL,pthread_producer,NULL);//创建生产者线程
	if(ret!=0)
	{
		sys_err("pthread_create error",ret);
	}
	ret=pthread_create(&tid2,NULL,pthread_consumer,NULL);//创建消费者线程
	if(ret!=0)
	{
		sys_err("pthread_create error",ret);
	}
	ret=pthread_join(tid1,NULL);//回收线程
	if(ret!=0)
        {
                sys_err("pthread_join error",ret);
        }
	ret=pthread_join(tid2,NULL);
	if(ret!=0)
        {
                sys_err("pthread_join error",ret);
        }
	ret=sem_destroy(&sem_space);//销毁信号量
	if(ret!=0)
        {
                sys_err("sem_destroy error",ret);
        }
	ret=sem_destroy(&sem_full);
	if(ret!=0)
        {
                sys_err("sem_destroy error",ret);
        }
	pthread_exit(NULL);
}
