#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<time.h>

#define DEFAULT_TIME 1 
#define MIN_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_VARY 10
#define true 1
#define false 0

void sys_err(const char* str)
{
	perror(str);
	exit(1);	
}

typedef struct
{
	void *(*function)(void*);         
	void *arg;                        
}threadpool_task_t;                  


typedef struct
{
	pthread_mutex_t lock;
	pthread_mutex_t thread_counter;
	pthread_cond_t queue_not_full;
	pthread_cond_t queue_not_empty;
	pthread_t threads;            
	pthread_t adjust_tid;           
	threadpool_task_t *task_queue;
	int min_thr_num;
	int max_thr_num;
	int live_thr_num;
	int busy_thr_num;
	int wait_exit_thr_num;
	int queue_front;
	int queue_rear;
	int queue_size;
	int queue_max_size;
	int shutdown;
}threadpool_t;
threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);
void* threadpool_thread(void *threadpool);
void *adjust_thread(void *threadpool);
int is_thread_alive(pthread_t tid);
int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void* arg);
void *process(void *arg);
int threadpool_destory(threadpool_t *pool);
int threadpool_free(threadpool_t *pool);

void* threadpool_thread(void *threadpool)
{
	threadpool_t *pool = (threadpool_t*)threadpool;
	threadpool_task_t task;
	while(1)
	{
		pthread_mutex_lock(&(pool->lock));
		while((pool->queue_size == 0)&&(!pool->shutdown))
		{
			printf("线程号为 0x%x 的线程正在阻塞等待任务......\n", (unsigned int)pthread_self());
			pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));
			printf("解除阻塞\n");
			if(pool->wait_exit_thr_num > 0)
			{
				pool->wait_exit_thr_num--;
				if(pool->live_thr_num > pool->min_thr_num)
				{
					printf("KO线程号为 0x%x 的线程被销毁\n", (unsigned int)pthread_self());
					pool->live_thr_num--;
					pthread_mutex_unlock(&(pool->lock));

					pthread_exit(NULL);
				}
			}
		}
		if (pool->shutdown) 
		{
			printf("--------shutdown == true\n");
			printf("KO线程号为 0x%x 的线程被销毁\n", (unsigned int)pthread_self());
			pool->live_thr_num--;
	    	pthread_mutex_unlock(&(pool->lock));
			pthread_exit(NULL);
		}
		task.function = pool->task_queue[pool->queue_front].function;
		task.arg = pool->task_queue[pool->queue_front].arg;
		pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
		pool->queue_size--;
		pthread_cond_broadcast(&(pool->queue_not_full));
		pthread_mutex_unlock(&(pool->lock));
		printf("++线程号为 0x%x 的线程开始工作\n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num++;//忙线程数+1
		pthread_mutex_unlock(&(pool->thread_counter));
		task.function(task.arg);

		printf("---线程号为 0x%x 的线程工作结束\n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num--;
		pthread_mutex_unlock(&(pool->thread_counter));
	}
	pthread_exit(NULL);
}

threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
	int i;
	threadpool_t *pool = NULL;
	do
	{
		if((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
		{
			printf("malloc threadpool fail\n");
			break;
		}

		pool->min_thr_num = min_thr_num;
		pool->max_thr_num = max_thr_num;
		pool->busy_thr_num = 0;
		pool->live_thr_num = min_thr_num;
		pool->wait_exit_thr_num = 0;
		pool->queue_size = 0;
		pool->queue_max_size = queue_max_size;
		pool->queue_front = 0;
		pool->queue_rear = 0;
		pool->shutdown = false;
		pool->task_queue = (threadpool_task_t*)malloc(sizeof(threadpool_task_t) * queue_max_size);
		if(pool->task_queue == NULL)
		{
			printf("malloc task_queue fail\n");
			break;
		}
		memset(pool->task_queue, 0 ,sizeof(threadpool_task_t) * queue_max_size);
		if(pthread_mutex_init(&(pool->lock), NULL) !=0 || pthread_mutex_init(&(pool->thread_counter),NULL) != 0 || pthread_cond_init(&(pool->queue_not_empty
			), NULL) != 0 || pthread_cond_init(&(pool->queue_not_full),NULL) != 0)
		{
			printf("init the lock or cond fail\n");
			break;
		}
		for(i = 0; i < min_thr_num; i++)
		{
			pthread_create(&(pool->threads), NULL, threadpool_thread, (void*)pool);
			pthread_detach(pool->threads);
			printf("～～线程号为 0x%x 的线程被创建\n",(unsigned int)pool->threads);
		}
		pthread_create(&(pool->adjust_tid), NULL, adjust_thread, (void*)pool);
		return pool;
	}
	while(0);
	threadpool_free(pool);
	return NULL;
}

void *adjust_thread(void *threadpool)
{
	int i;
	threadpool_t *pool = (threadpool_t*)threadpool;
	while(!pool->shutdown)
	{
		sleep(DEFAULT_TIME);
		pthread_mutex_lock(&(pool->lock));
		int queue_size = pool->queue_size;
		int live_thr_num = pool->live_thr_num;
		pthread_mutex_unlock(&(pool->lock));
		pthread_mutex_lock(&(pool->thread_counter));
		int busy_thr_num = pool->busy_thr_num;
		pthread_mutex_unlock(&(pool->thread_counter));
		if(queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < pool->max_thr_num)
		{
			pthread_mutex_lock(&(pool->lock));
			for(i = 0; i < DEFAULT_THREAD_VARY && pool->live_thr_num < pool->max_thr_num; i++)
			{
				pthread_create(&(pool->threads), NULL, threadpool_thread, (void*)pool);
				pthread_detach(pool->threads);
				pool->live_thr_num++;
			}
			printf("---扩容--存活线程数为 %d\n", pool->live_thr_num);

			pthread_mutex_unlock(&(pool->lock));
		}
		if((busy_thr_num * 2) < live_thr_num && live_thr_num > pool->min_thr_num)
		{
			pthread_mutex_lock(&(pool->lock));
			pool->wait_exit_thr_num = DEFAULT_THREAD_VARY;
			for(i = 0; i< DEFAULT_THREAD_VARY; i++)
			{
				pthread_cond_signal(&(pool->queue_not_empty));
			}
			sleep(1);
			printf("--缩容-存活线程数为 %d\n", pool->live_thr_num);
			
		}

	}
	return NULL;	
}

int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void* arg)
{
	pthread_mutex_lock(&(pool->lock));
	while((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
	{
		pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
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
	pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
	pool->queue_size++;
	pthread_cond_signal(&(pool->queue_not_empty));
	pthread_mutex_unlock(&(pool->lock));
	return 0;
}

int threadpool_destory(threadpool_t *pool)
{
	if(pool == NULL)
	{
		return -1;
	}
	pool->shutdown = true;
	pthread_join(pool->adjust_tid, NULL);
	pthread_cond_broadcast(&(pool->queue_not_empty));	
	printf("--------线程池销毁即将完毕\n");
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

int threadpool_free(threadpool_t *pool)
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
#if 0
int is_thread_alive(pthread_t tid)
{
	int kill_rc = pthread_kill(tid, 0);
	if (kill_rc == ESRCH) 
	{
		return 0;
	}
	return 1;
}
#endif

void *process(void *arg)
{
	printf("线程号为 0x%x 的线程正在处理数据 %d\n ",(unsigned int)pthread_self(),*(int *)arg);
	usleep((rand()%50+1)*1000);
	printf("数据 %d 处理完毕\n",*(int *)arg);
	return NULL;
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	threadpool_t *thp = threadpool_create(5,100,100);
	printf("pool inited\n");
	int num[2048];
	int i;
	for(i = 0; i < 2048; i++)
	{
		if(i % 500 == 100)
		{
			sleep(1);
		}
		num[i] = i;
		printf("><生产出数据 %d\n", i);
		threadpool_add(thp, process, (void*)&num[i]);
	}
	sleep(20);
	printf("------存活线程数为：%d\n", thp->live_thr_num);
	printf("--------即将销毁线程池------\n");
	threadpool_destory(thp);
	sleep(20);
	pthread_exit(NULL);
}
