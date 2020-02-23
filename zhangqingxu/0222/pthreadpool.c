#include<stdio.h>                                                           
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#include<time.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<semaphore.h>

#define DEFAULT_TIME 1  //每1秒检测一次
#define MIN_WAIT_TASK_NUM 10  //最小事件个数
#define DEFAULT_THREAD_VARY 10   //一次创建/销毁的线程数
#define true 1
#define false 0
typedef struct
{
	void* (*function) (void*);    //函数指针
	void* arg;	        //function的参数
}threadpool_task_t;

typedef struct
{
	pthread_mutex_t lock;      //锁住除了忙线程数，本结构体其他信息
	pthread_mutex_t thread_counter;  //锁住忙线程数

	pthread_cond_t queue_not_full;      //任务队列满时，添加任务的线程阻塞等待此信号量
	pthread_cond_t queue_not_empty;     //任务队列为空，线程等待此信号量

	pthread_t threads;         //临时存放每个线程的id
	pthread_t adjust_tid;      //存放管理线程的id
	threadpool_task_t *task_queue;  //任务队列

	int min_thr_num;    //线程池最小线程数
	int max_thr_num;    //最大线程数
	int live_thr_num;    //存活线程数
	int busy_thr_num;    //忙线程数
	int wait_exit_thr_num;   //将要销毁的线程数

	int queue_front;     //任务队列的头下标，记录将要读取数据的位置
	int queue_rear;      //尾下标，记录将要写入信息的位置
	int queue_size;      //实际任务数
	int queue_max_size;    //可容纳的最大任务数
	int shutdown;     //标志尾，线程池使用状态
}threadpool_t;
//创建，初始化线程池
threadpool_t * threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size);

//线程池线程的主函数  （工作线程的回调函数）
void* threadpool_thread(void* threadpool);
//管理者线程的主函数
void* adjust_thread(void* threadpool);
//向线程池添加一个任务
int threadpool_add(threadpool_t *pool,void*(function)(void* arg),void* arg);
//数据处理方法
void* process(void* arg);
//销毁线程池
int threadpool_destory(threadpool_t *pool);
//释放线程空间
int threadpool_free(threadpool_t* pool);


//创建，初始化线程池
threadpool_t * threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size)
{
	threadpool_t * pool = NULL;
	do
	{
		if((pool = (threadpool_t*)malloc(sizeof(threadpool_t)))==NULL)
		{
			printf("pool malloc fail\n");
			break;
		}
		pool->min_thr_num = min_thr_num;
		pool->max_thr_num = max_thr_num;
		pool->live_thr_num = min_thr_num;
		pool->busy_thr_num = 0;
		pool->wait_exit_thr_num = 0;
		pool->queue_max_size = queue_max_size;
		pool->queue_front = 0;
		pool->queue_rear = 0;
		pool->shutdown = false;
		pool->queue_size = 0;

		//创建任务队列，在结构体中定义，在这里分配空间
		pool->task_queue = (threadpool_task_t*)malloc(sizeof(threadpool_task_t)*queue_max_size);
		if(pool->task_queue == NULL)
		{
			printf("queue malloc fail\n");
			break;
		}
		memset(pool->task_queue,0,sizeof(threadpool_task_t)*queue_max_size); //初始化任务队列
		//初始化两把锁（互斥量），两个信号量
		if(pthread_mutex_init(&(pool->lock),NULL) !=0 || pthread_mutex_init(&(pool->thread_counter),NULL)!=0 || pthread_cond_init(&(pool->queue_not_full),NULL) || pthread_cond_init(&(pool->queue_not_empty),NULL)!=0)
		{
			printf("mutex or cond is fail\n");
			break;
		}
		for(int i = 0;i<min_thr_num;i++)    //创建最基本的线程数
		{
			pthread_create(&(pool->threads),NULL,threadpool_thread,(void*)pool);
			pthread_detach(pool->threads);
			printf("线程号为: 0x%x 的线程被创建\n",(unsigned int )pool->threads);
		}
		pthread_create(&(pool->adjust_tid),NULL,adjust_thread,(void*)pool);  //创建管理线程
		return pool;
	}while(0);
	threadpool_free(pool);
	return NULL;
}


//线程池线程主函数
void* threadpool_thread(void* threadpool)
{
	threadpool_t* pool = (threadpool_t*)threadpool;
	threadpool_task_t task;
	while(1)
	{
		pthread_mutex_lock(&(pool->lock));
		while(pool->queue_size == 0 && (!pool->shutdown))       //管理线程 消除线程用这个
		{
			printf("线程号为: 0x%x 的线程阻塞等待!!!\n",(unsigned int )pthread_self());
			pthread_cond_wait(&(pool->queue_not_empty),&(pool->lock));
			printf("解除阻塞\n");

			if(pool->wait_exit_thr_num>0)
			{
				pool->wait_exit_thr_num--;

				if(pool->live_thr_num > pool->min_thr_num)
				{
					printf("-KO-线程号为 0x%x 的线程被销毁\n", (unsigned int)pthread_self());
					pool->live_thr_num--;
					pthread_mutex_unlock(&(pool->lock));

					pthread_exit(NULL);
				}
			}
		}

		if(pool->shutdown)     //最终销毁用这个
		{
				printf("-----------------------------------------------shutdown==true\n");
				printf("-KO-线程号为 0x%x 的线程被销毁\n", (unsigned int)pthread_self());
				pool->live_thr_num--;
				pthread_mutex_unlock(&(pool->lock));
				pthread_exit(NULL);
		}
		
		//从线程池中获得任务
		task.function = pool->task_queue[pool->queue_front].function;
		task.arg = pool->task_queue[pool->queue_front].arg;

		//将读取线程的位置下标后移
		pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
		
		//任务数-1
		pool->queue_size--;
		
		//唤醒  等待添加任务的线程
		pthread_cond_broadcast(&(pool->queue_not_full));

		pthread_mutex_unlock(&(pool->lock));

		printf("+++线程号为 0x%x 的线程开始工作\n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num++;                       //忙线程加一
		pthread_mutex_unlock(&(pool->thread_counter));

		task.function(task.arg);   //回调函数


		printf("--------线程号为 0x%x 的线程工作结束\n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num--;                       //忙线程减一
		pthread_mutex_unlock(&(pool->thread_counter));
	}
	pthread_exit(NULL);
}

//管理线程主函数
void* adjust_thread(void* threadpool)
{
	threadpool_t * pool = (threadpool_t*)threadpool;
	while(!pool->shutdown)
	{
		sleep(DEFAULT_TIME);            //定时检查

		pthread_mutex_lock(&(pool->lock));
		int queue_size = pool->queue_size;
		int live_thr_num = pool->live_thr_num;
		int max_thr_num = pool->max_thr_num;
		int min_thr_num = pool->min_thr_num;
		pthread_mutex_unlock(&(pool->lock));

		pthread_mutex_lock(&(pool->thread_counter));
		int busy_thr_num = pool->busy_thr_num;
		pthread_mutex_unlock(&(pool->thread_counter));
        //创建线程 ---任务数大于最小任务数 ， 并且存活线程小于最大线程数
		if((queue_size > MIN_WAIT_TASK_NUM) && live_thr_num < max_thr_num)
		{
			pthread_mutex_lock(&(pool->lock));
			for(int i = 0;i<DEFAULT_THREAD_VARY && pool->live_thr_num < pool->max_thr_num;i++)
			{
				pthread_create(&(pool->threads),NULL,threadpool_thread,(void*)pool);
				pthread_detach(pool->threads);
				pool->live_thr_num++;
			}

			printf("----------------------扩容---------------------存活线程数为：%d\n", pool->live_thr_num);

			pthread_mutex_unlock(&(pool->lock));
		}
		//销毁线程  ---忙线程*2 小于存活线程 ， 并且存活线程大于最小线程数
		if((busy_thr_num * 2) < live_thr_num && live_thr_num > min_thr_num)
		{
			pthread_mutex_lock(&(pool->lock));
			pool->wait_exit_thr_num = DEFAULT_THREAD_VARY;    //设置销毁线程数
			pthread_mutex_unlock(&(pool->lock));

			for(int i = 0;i<DEFAULT_THREAD_VARY;i++)
			{
				pthread_cond_signal(&(pool->queue_not_empty));
			}
			sleep(1);
			printf("----------------------缩容---------------------存活线程数为：%d\n", pool->live_thr_num);
		}
	}

	return NULL;
}


int threadpool_add(threadpool_t *pool,void*(function)(void* arg),void* arg)
{
	pthread_mutex_lock(&(pool->lock));
	while((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
	{
		pthread_cond_wait(&(pool->queue_not_full),&(pool->lock));
	}
	if(pool->shutdown)
	{
		pthread_cond_broadcast(&(pool->queue_not_empty));
		pthread_mutex_unlock(&(pool->lock));
		return 0;
	}

	if(pool->task_queue[pool->queue_rear].arg != NULL)
	{
		pool->task_queue[pool->queue_rear].arg = NULL;
	}

	pool->task_queue[pool->queue_rear].function = function;
	pool->task_queue[pool->queue_rear].arg = arg;
	pool->queue_rear = (pool->queue_rear + 1) %pool->queue_max_size;
	pool->queue_size++;

	pthread_cond_signal(&(pool->queue_not_empty));
	pthread_mutex_unlock(&(pool->lock));

	return 0;
}


void* process(void* arg)
{
	printf("...线程号为 0x%x 的线程正在处理数据 %d\n ",(unsigned int)pthread_self(),*(int *)arg);
	usleep((rand()%50 + 1)*1000);
	printf("数据 %d 处理完毕\n",*(int *)arg);
	return NULL;
}


int threadpool_destory(threadpool_t *pool)
{
	if(pool == NULL)
	{
		return -1;
	}
	pool->shutdown = true;

	pthread_join(pool->adjust_tid,NULL);

	pthread_cond_broadcast(&(pool->queue_not_empty));
	printf("----------------------线程池销毁即将完毕\n");
	while(1)
	{
		pthread_mutex_lock(&(pool->lock));
		if(pool->live_thr_num == 0)
		{
			pthread_mutex_unlock(&(pool->lock));
			break;
		}
		else
		{
			printf("正在销毁线程池，请稍等...\n");
			pthread_mutex_unlock(&(pool->lock));
			sleep(1);
		}
	}
	printf("线程池销毁完毕，即将释放内存空间\n");
	threadpool_free(pool);
	return 0;
}


int threadpool_free(threadpool_t* pool)
{
	if(pool == NULL)
	{
		return -1;
	}
	if(pool->task_queue)
	{
		free(pool->task_queue);
	}
	pthread_mutex_lock(&(pool->lock));
	pthread_mutex_destroy(&(pool->lock));
	pthread_mutex_lock(&(pool->thread_counter));
	pthread_mutex_destroy(&(pool->thread_counter));
	pthread_cond_destroy(&(pool->queue_not_empty));
	pthread_cond_destroy(&(pool->queue_not_full));
	free(pool);
	pool = NULL;
	printf("内存空间释放完毕\n");
	return 0;
}


int main(int argc, char* argv[])
{
	srand(time(NULL));
	//先创建一个线程池
	threadpool_t* pth = threadpool_create(5,100,100);
	printf("pool made  \n");

	int num[2048];
	for(int i = 0; i < 2048;i++)
	{
		if(i%500 == 100)
		{
			sleep(1);
		}
		num[i] = i;
		printf("><生产出数据 %d\n", i);
		threadpool_add(pth,process,(void*)&num[i]);
	}
	sleep(20);
	printf("----------------存活线程数为：%d\n", pth->live_thr_num);
	printf("----------------即将销毁线程池---------------------\n");
	threadpool_destory(pth);

	sleep(20);
	pthread_exit(NULL);
}

