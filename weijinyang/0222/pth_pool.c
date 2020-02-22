#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define DEFAULT_TIME 1
#define MIN_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_VARY 10
#define true 1
#define false 0
typedef struct
{
    void *(*function)(void*);   //函数指针
    void *arg;                  //函数参数
}threadpool_task_t;             //任务结构体，存任务数据和处理方式

typedef struct
{
    //两个锁
    pthread_mutex_t lock;      //锁住除了忙线程外的锁
    pthread_mutex_t thread_counter;//锁忙线程数
    //两个条件变量
    pthread_cond_t queue_not_full; //队列未满，满时添加任务线程阻塞，等待此条件变量满足
    pthread_cond_t queue_not_empty;//队列未空，任务队列中含有任务，通知处理任务线程处理任务（也是个条件变量）

    pthread_t threads;             //线程池中线程的 tid
    pthread_t adjust_tid;          //管理线程 的tid
    threadpool_task_t *task_queue; //任务队列

    int min_thr_num;           //线程池中最小存在的线程数
    int max_thr_num;           //线程池中最大容纳的线程数
    int live_thr_num;          //线程池存活线程数
    int busy_thr_num;          //线程池忙线程数
    int wait_exit_thr_num;     //等待销毁的线程数

    int queue_front;           //task_queue队列的队头下标
    int queue_rear;	       //task_queue队列的队尾下标
    int queue_size;	       //任务队列当前的任务数
    int queue_max_size;        //任务队列可容纳的最大任务数
    int shutdown;              //标志位,true false 标志线程是否需要被杀死
}threadpool_t;



threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);

void* threadpool_thread(void *threadpool);

void *adjust_thread(void *threadpool);

int is_thread_alive(pthread_t tid);

int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void* arg);

void *process(void *arg);

int threadpool_destory(threadpool_t *pool);

int threadpool_free(threadpool_t *pool);

//处理任务线程的回调函数
void *threadpool_thread(void *threadpool)
{
    threadpool_t *pool = (threadpool_t*)threadpool;//转变类型
    threadpool_task_t task;//临时变量，用来存放取出的任务

    while(1)
    {
	pthread_mutex_lock(&(pool->lock));//访问共享区域  先加锁
	//当前队列没有任务 并且 线程池存在
	while((pool->queue_size == 0) && (!pool->shutdown))
	{
	    printf("---有线程正阻塞等待任务，线程号为%x---\n", (unsigned int)pthread_self());
	    //阻塞 并解锁
	    pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));

	    printf("------阻塞被解除了！！------\n");
	    //清楚 空闲线程  wait_exit_thr_num 个
	    if(pool->wait_exit_thr_num > 0)
	    {
		pool->wait_exit_thr_num--;
		//判断 线程池里线程是否大于最小线程个数
		if(pool->live_thr_num > pool->min_thr_num)
		{
		    printf("---有一个线程被杀死---线程号为---%x\n", (unsigned int)pthread_self());
		    pool->live_thr_num--;
		    pthread_mutex_unlock(&(pool->lock));

		    pthread_exit(NULL);
		}
	    }
	}
	//如果 shutdown为true 关闭所有 自退出
	if(pool->shutdown)
	{
	    printf("-------shutdown is true-------\n");
	    printf("---一个线程被销毁---线程号为---%x---\n", (unsigned int)pthread_self());
	    pool->live_thr_num--;
	    pthread_mutex_unlock(&(pool->lock));
	    pthread_exit(NULL);
	}
	
	//处理 任务,把任务拿出来 放进临时变量
	task.function = pool->task_queue[pool->queue_front].function;
	task.arg = pool->task_queue[pool->queue_front].arg;
	
	pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;//对头下标后移
	pool->queue_size--;//任务数 --

	//通知可以有新的任务添加
	pthread_cond_broadcast(&(pool->queue_not_full));

	//任务出队列 解锁
	pthread_mutex_unlock(&(pool->lock));

	//执行任务
	printf("----一个线程开始工作---线程号为---%x---\n", (unsigned int)pthread_self());
	pthread_mutex_lock(&(pool->thread_counter));//改变忙线程数量先加锁
	pool->busy_thr_num++;
	pthread_mutex_unlock(&(pool->thread_counter));

	task.function(task.arg);//执行回调函数

	//任务处理结束
	printf("----一个线程工作结束---线程号为--%x---\n", (unsigned int)pthread_self());
	pthread_mutex_lock(&(pool->thread_counter));
	pool->busy_thr_num--;
	pthread_mutex_unlock(&(pool->thread_counter));
    }
    pthread_exit(NULL);
}
//管理线程回调函数
void *adjust_thread(void *threadpool)
{
    int i;
    threadpool_t *pool = (threadpool_t*)threadpool;
    while(!pool->shutdown)
    {
	sleep(DEFAULT_TIME);//定时对线程池进行管理

	pthread_mutex_lock(&(pool->lock));//加锁
	int queue_size = pool->queue_size;//取任务数
	int live_thr_num = pool->live_thr_num;//取存活线程数
	pthread_mutex_unlock(&(pool->lock));

	pthread_mutex_lock(&(pool->thread_counter));
	int busy_thr_num = pool->busy_thr_num;//取忙线程数
	pthread_mutex_unlock(&(pool->thread_counter));

	//创建新线程，队列中任务数大于最小任务个数并且存活的线程数不能超过最大线程数
	if(queue_size >= MIN_WAIT_TASK_NUM && live_thr_num < pool->max_thr_num)
	{
	    //加锁
	    pthread_mutex_lock(&(pool->lock));
	    //增加线程  一次增加DEFAULT_THREAD_VARY个
	    for(i = 0; i < DEFAULT_THREAD_VARY && pool->live_thr_num < pool->max_thr_num; i++)
	    {
		pthread_create(&(pool->threads), NULL, threadpool_thread, (void*)pool);
		pthread_detach(pool->threads);
		pool->live_thr_num++;
	    }
	    printf("---增加线程---%d个---扩容后线程数为---%d个\n", i, pool->live_thr_num);
	    
	    pthread_mutex_unlock(&(pool->lock));
	}
	
	
	//销毁空闲线程，存活线程数大于最小线程数 忙线程*2 小于存活线程数
	if((busy_thr_num * 2) < live_thr_num && live_thr_num > pool->min_thr_num)
	{
	    //一次行销毁10个  设置pool->wait_exit_thr_num
	    pthread_mutex_lock(&(pool->lock));
	    pool->wait_exit_thr_num = DEFAULT_THREAD_VARY;
	    pthread_mutex_unlock(&(pool->lock));

	    for(i = 0; i < DEFAULT_THREAD_VARY; i++)
	    {
		//发送信号， 唤醒阻塞在 queue_not_empty上的线程，让他们自行终止
		pthread_cond_signal(&(pool->queue_not_empty));
	    }
	    sleep(1);
	    printf("---减少线程---%d个---减少后线程数为---%d个\n", i, pool->live_thr_num);

	}
    }

    return NULL;
}

//向线程中添加任务
int threadpool_add(threadpool_t *pool, void *(*function)(void *arg), void *arg)
{
    pthread_mutex_lock(&(pool->lock));
    
    //队列已满 阻塞
    while((pool->queue_size == pool->queue_max_size) && (!pool->shutdown))
    {
	pthread_cond_wait(&(pool->queue_not_full), &(pool->lock));
    }

    if(pool->shutdown)
    {
	//唤醒阻塞在 pool->queue_not_empty上的线程
	pthread_cond_broadcast(&(pool->queue_not_empty));
	pthread_mutex_unlock(&(pool->lock));
	return 0;
    }

    //清空arg
    if(pool->task_queue[pool->queue_rear].arg != NULL)
    {
	pool->task_queue[pool->queue_rear].arg = NULL;
    }

    //添加任务到对列
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;//对尾指针后移
    pool->queue_size++;

    //添加完成后，唤醒在阻塞在线程池中等待处理数据的线程
    pthread_cond_signal(&(pool->queue_not_empty));
    pthread_mutex_unlock(&(pool->lock));

    return 0;
}

//线程池创建函数，参数，线程池最小，最大线程数 队列最大任务数
threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size)
{
    int i;
    threadpool_t *pool = NULL;//定义一个线程池结构体
    do//do while  如果出错直接跳到最后
    {
	//给线程池 在堆区 malloc申请一块空间，如果返回空 就出错
	if((pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL)
	{
	    printf("malloc threadpool failed. \n");
	    break;
	}
	//初始化线程池
	pool->min_thr_num = min_thr_num;
	pool->max_thr_num = max_thr_num;
	pool->busy_thr_num = 0;//忙线程数初始化为0
	pool->live_thr_num = min_thr_num;//存活线程数为最小线程数
	pool->wait_exit_thr_num = 0;//等待销毁线程数初始化为0
	
	pool->queue_size = 0;//当前任务数为0
	pool->queue_max_size = queue_max_size;//最大任务数赋值
	pool->queue_front = 0;//对头下标为0
	pool->queue_rear = 0;//队尾下标为0
	pool->shutdown = false;//不关闭线程池

	//给任务队列 开辟空间
	pool->task_queue = (threadpool_task_t*)malloc(sizeof(threadpool_task_t) * queue_max_size);
	if(pool->task_queue == NULL)
	{
	    printf("malloc task_queue failed. \n");
	    break;
	}
	//队列初始化为0
	memset(pool->task_queue, 0, sizeof(threadpool_task_t) * queue_max_size);
	//初始化互斥锁和条件变量
	if(pthread_mutex_init(&(pool->lock), NULL) != 0 || pthread_mutex_init(&(pool->thread_counter), NULL) != 0 || pthread_cond_init(&(pool->queue_not_full), NULL) != 0 || pthread_cond_init(&(pool->queue_not_empty), NULL) != 0)
	{
	    printf("lock or cond init failed. \n");
	    break;
	}

	//创建线程池初始化线程 最小线程数个
	for(i = 0; i < min_thr_num; i++)
	{
	    pthread_create(&(pool->threads), NULL, threadpool_thread, (void*)pool);
	    pthread_detach(pool->threads);
	    printf("---创建了新的线程，线程号为%x---\n", (unsigned int)pool->threads);
	}
	//创建管理线程，管理其他线程
	pthread_create(&(pool->adjust_tid), NULL, adjust_thread, (void*)pool);
	return pool;//将线程池返回给主函数
    }
    while(0);
    threadpool_free(pool);//一旦上面出现错误，释放线程池pool的空间
    return NULL;
}
//销毁线程池
int threadpool_destory(threadpool_t *pool)
{
    if(pool == NULL)
    {
	return -1;
    }
    pool->shutdown = true;

    //销毁管理线程
    pthread_join(pool->adjust_tid, NULL);

    pthread_cond_broadcast(&(pool->queue_not_empty));
    printf("--------------------线程池销毁即将成功\n");

    while(1)//有线程正在处理数据 没有被杀死 循环查找
    {
	pthread_mutex_lock(&(pool->lock));
	if(pool->live_thr_num == 0)
	{
	    pthread_mutex_unlock(&(pool->lock));
	    break;
	}
	else
	{
	    printf("正在销毁线程池-------\n");
	    pthread_mutex_unlock(&(pool->lock));
	    sleep(1);

	}
    }
    printf("---线程池销毁完毕，开始释放空间---\n");
    threadpool_free(pool);
    return 0;
}
//释放空间
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
    pthread_mutex_unlock(&(pool->thread_counter));

    pthread_cond_destroy(&(pool->queue_not_empty));
    pthread_cond_destroy(&(pool->queue_not_full));

    free(pool);
    pool = NULL;
    printf("-----空间释放完成-----\n");
    return 0;
}
//数据处理
void *process(void *arg)
{
    printf("-------一个线程正在工作---线程号为---%x---数据为---%d---\n", (unsigned int)pthread_self(), *(int *)arg);
    usleep((rand()%50+1)*1000);
    printf("---线程处理完毕---线程号---%x---数据---%d---\n", (unsigned int)pthread_self(), *(int *)arg);

    return NULL;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    //创建线程池
    threadpool_t *thp = threadpool_create(5, 100, 100);
    printf("---pool create success.---\n");

    int num[2048];
    int i;
    for(i = 0; i < 2048; i++)
    {
	if(i % 500 == 100)
	{
	    sleep(1);
	}
	num[i] = i;
	printf("---生产数据---%d---\n", i);
	threadpool_add(thp, process, (void*)&num[i]);
    }
    sleep(20);//等待子线程完成任务
    printf("------------------当前存活线程数为-%d\n", thp->live_thr_num);
    printf("------------------即将销毁线程池---------------\n");
    threadpool_destory(thp);
    sleep(20);
    pthread_exit(NULL);

    return 0;
}

