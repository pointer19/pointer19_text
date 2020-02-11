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
    exit(1);//1 非正常退出 0  正常退出
}
int i = 10;
void* pthread_fun(void * arg)//线程主函数
{
    int j = (int)arg;
    printf("j = %d,pid = %d, tid = %lu\n", j, getpid(), pthread_self());
    return NULL;
}
int main(int argc, char* argv[]) //主线程
{
    pthread_t tid1;//创建线程
    pthread_create(&tid1, NULL, pthread_fun, (void *)i);
    sleep(1);
    return 0;
}

