#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void sys_err(const char* str,int errno)
{
    fprintf(stderr, "%s: %s", str, strerror(errno));
    exit(1);
}

void* pthread_fun(void * arg)
{
    sleep(1);
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    return NULL;
}
int main(int argc, char* argv[])//主线程
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, pthread_fun, NULL);
    pthread_detach(tid1);//线程分离
    int val = pthread_join(tid1, NULL);
    if(val == 0)
    {
	printf("回收线程\n");
    }
    pthread_exit(NULL); 
}

