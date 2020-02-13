#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;

struct food
{
	int num;
	struct food* next;	
};

struct food* head=NULL;

void* pthread_make(void * arg)
{
	while(1)
	{
		struct food* make;
		make=(struct food *)malloc(sizeof(struct food));
		make->num=rand()%100+1;
		
		printf("------make a food, num=%d\n",make->num);
		pthread_mutex_lock(&mutex);//加锁
		make->next=head;//放到共享区
		head=make;
		
		pthread_mutex_unlock(&mutex);//解锁
		pthread_cond_signal(&cond);//唤醒阻塞在wait，放数据后调用，wait就解除阻塞 拿锁

		sleep(rand()%5);
	}
	return NULL;
}
void* pthread_eat(void * arg)
{
	while(1)
	{
		struct food *eat;
		pthread_mutex_lock(&mutex);//加锁
			if(head==NULL)//看里面有没有数据
			{
				pthread_cond_wait(&cond,&mutex);//阻塞、释放锁（原子操作）
			}
		eat=head;//吃数据
		head=head->next;
		pthread_mutex_unlock(&mutex);//解锁
		
		printf("--------eat the food,--------num=%d\n",eat->num);
		
		free(eat);//吃掉节点后要释放
		sleep(rand()%5);
	}
	return NULL;
		
}
int main(int argc,char* argv[])
{
	srand(time(NULL));

	pthread_t tid1,tid2;
	pthread_mutex_init(&mutex,NULL);//初始化
	pthread_cond_init(&cond,NULL);

	pthread_create(&tid1,NULL,pthread_make,NULL);//创建
	pthread_create(&tid2,NULL,pthread_eat,NULL);

	pthread_join(tid1,NULL);//回收
	pthread_join(tid2,NULL);
	
	pthread_mutex_destroy(&mutex);//回收
	pthread_cond_destroy(&cond);


	pthread_exit(NULL);
	return 0;	
}

