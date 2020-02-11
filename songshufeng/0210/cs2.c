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

void * fun1()
{
    exit(0);
    //return NULL;
}
void* pthread_fun(void * arg)//线程主函数
{printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    fun1();
    printf("ssssssssssss\n");
    return NULL;
}
int main(int argc, char* argv[])//主线程
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, pthread_fun, NULL);
    sleep(1);
    printf("songgggggggggg");
    return 0;//结束主线程，exit 0也可以
}

