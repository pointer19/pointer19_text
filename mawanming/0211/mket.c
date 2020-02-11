#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
struct food//共享区域
{
	int nu;
	struct food*next;//链表
};
struct food* head=NULL;//链表头节点
void* pthread_make(void* arg)//生产者线程主函数
{
	while(1)
	{
		struct food* make;//创建结构体
		make=(struct food*)malloc(sizeof(struct food));//分配空间
		make->nu=rand()%500+1;
		printf("------make a food, un=%d\n",make->nu);
		pthread_mutex_lock(&mutex);//加锁
		make->next=head;//头插法
		head=make;
		pthread_mutex_unlock(&mutex);//释放锁
		pthread_cond_signal(&cond);//唤醒条件变量阻塞的线程
		sleep(rand()%5);
	}
	return NULL;
}
void* pthread_eat(void* arg)//消费者线程主函数
{
	while(1)
	{
		struct food* eat;
		pthread_mutex_lock(&mutex);//加锁
		if(head==NULL)//无数据则条件变量阻塞等待
		{
			pthread_cond_wait(&cond,&mutex);
		}
		eat=head;//被唤醒后申请加锁
		head=head->next;
		pthread_mutex_unlock(&mutex);//公共数据修改完毕，解除锁
		printf("eat a food, nu=%d\n",eat->nu);
		free(eat);//释放结构体
		sleep(rand()%5);
	}
	return NULL;
}
int main(int argc,char* argv[])
{
	pthread_t tid1,tid2;//线程id
	srand(time(NULL));//随机数种子
	pthread_mutex_init(&mutex,NULL);//锁初始化
	pthread_cond_init(&cond,NULL);//条件变量初始化
	pthread_create(&tid1,NULL,pthread_make,NULL);//创建线程
	pthread_create(&tid2,NULL,pthread_eat,NULL);
	pthread_join(tid1,NULL);//阻塞回收线程
	pthread_join(tid2,NULL);
	pthread_mutex_destroy(&mutex);//销毁锁
	pthread_cond_destroy(&cond);//销毁条件变量
	pthread_exit(NULL);
}
