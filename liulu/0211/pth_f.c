#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define H 10000
int x = 1;
void sys_err(const char* str,int errno)
{
//    strerror(errno);//fan yi  cuo wu hao
    fprintf(stderr, "%s: %s", str, strerror(errno));
    pthread_exit(NULL);
}
pthread_mutex_t mutex;
void* pthread_fun(void * arg)
{//zi  xian  cheng
    for(int i = 0; i < H; i++)
    {
	pthread_mutex_lock(&mutex);
	printf("%d\n", x++);
	pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
int main(int argc, char* argv[])
{ 
    pthread_t tid;
    pthread_mutex_init(&mutex, NULL);
    int retval = pthread_create(&tid, NULL, pthread_fun, NULL);
    if(retval != 0)
    {
	sys_err("pthread_create error", retval);
    }
    for(int i = 0; i < H; i++)
    {
	pthread_mutex_lock(&mutex);
	printf("%d\n", x++);
	pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}

