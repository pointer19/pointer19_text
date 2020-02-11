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
    exit(1);//1 �������˳� 0  �����˳�
}
int i = 10;
void* pthread_fun(void * arg)//�߳�������
{
    int j = (int)arg;
    printf("j = %d,pid = %d, tid = %lu\n", j, getpid(), pthread_self());
    return NULL;
}
int main(int argc, char* argv[]) //���߳�
{
    pthread_t tid1;//�����߳�
    pthread_create(&tid1, NULL, pthread_fun, (void *)i);
    sleep(1);
    return 0;
}

