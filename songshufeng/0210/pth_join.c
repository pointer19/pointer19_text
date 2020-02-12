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

void* pthread_fun(void * arg)//线程主函数，子线程
{
    sleep(2);
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    return (void*)100;
}
int main(int argc, char* argv[])//主线程
{
    pthread_t tid1;
    int *retval;
    pthread_create(&tid1, NULL, pthread_fun, NULL);
    pthread_join(tid1,(void**)&retval);//int **转化为void **
    printf("retval = %d\n", (int *)retval);
    pthread_exit(NULL); 
}

