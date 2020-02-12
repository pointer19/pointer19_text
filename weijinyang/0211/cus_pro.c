#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
pthread_mutex_t mutex;
pthread_cond_t cond;

struct shop
{
    int nu;
    struct shop* next;
};

struct shop* head = NULL;
void* pthread_pro(void* arg)
{
    while(1)
    {
	struct shop* thing;
	thing = (struct shop*)malloc(sizeof(struct shop));
	thing->nu = rand()%200+1;
	printf("生产了商品：%d\n", thing->nu);
	
	pthread_mutex_lock(&mutex);
	thing->next = head;
	head = thing;
	pthread_mutex_unlock(&mutex);

	pthread_cond_signal(&cond);

	sleep(rand()%6);
    }
    return NULL;
}
void* pthread_cus(void* arg)
{
    while(1)
    {
	struct shop* things;
	pthread_mutex_lock(&mutex);
	if(head == NULL)
	{
	    pthread_cond_wait(&cond, &mutex);
	}
	
	things = head;
	head = head->next;
	pthread_mutex_unlock(&mutex);
	printf("消费者购买了：%d\n", things->nu);

	free(things);
	sleep(rand()%6);
    }
    return NULL;
}
int main(int argc, char* argv[])
{
    srand(time(NULL));
    pthread_t tid1, tid2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&tid1, NULL, pthread_pro, NULL);
    pthread_create(&tid2, NULL, pthread_cus, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

