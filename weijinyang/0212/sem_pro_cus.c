#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
void sys_err(char* str, int no)
{
    fprintf(stderr, "%s:%s\n", str, strerror(no));
    exit(1);
}
#define N 10

int arry[N] = {0};

sem_t sem_space;
sem_t sem_full;

void* pthread_pro(void* arg)
{
    int i = 0;//  biao shi shu zu  xia biao
    while(1)
    {
	sem_wait(&sem_space);
	arry[i] = rand() % 500 + 1;
	printf("pro a shop no = %d\n", arry[i]);
	sem_post(&sem_full);

	i = (i + 1) % N;
	sleep(rand() % 3);
    }
    return NULL;
}
void* pthread_cus(void* arg)
{
    int i = 0;
    while(1)
    {
	sem_wait(&sem_full);
	printf("cus a shop no = %d\n", arry[i]);
	arry[i] = 0;
	sem_post(&sem_space);

	i = (i + 1) % N;
	sleep(rand() % 3);
    }
    return NULL;
}
int main(int argc, char* argv[])
{
    pthread_t tid1, tid2;
    int ret;
    srand(time(NULL));
    sem_init(&sem_space, 0, N);
    sem_init(&sem_full, 0, 0);
    ret = pthread_create(&tid1, NULL, pthread_pro, NULL);
    if(ret != 0)
    {
	sys_err("pthread_create_pro err", ret);
    }
    ret = pthread_create(&tid2, NULL, pthread_cus, NULL);
    if(ret != 0)
    {
	sys_err("pthread_create_cus err", ret);
    }
    
    ret = pthread_join(tid1, NULL);
    if(ret != 0)
    {
	sys_err("pthread_join_pro err", ret);
    }
    ret = pthread_join(tid2, NULL);
    if(ret != 0)
    {
	sys_err("pthread_join_cus err", ret);
    }

    sem_destroy(&sem_space);
    sem_destroy(&sem_full);
    pthread_exit(NULL);
}

