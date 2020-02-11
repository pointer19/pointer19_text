#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

void sys_err(char* str, int nu)
{
    fprintf(stderr, "%s:%s\n", str,strerror(nu));
    pthread_exit(NULL);
}

void* pthread_fun(void* arg)
{
    printf("this is child pth\n");
    return NULL;
}
int main(int argc, char* argv[])
{
    pthread_t tid;
    
    pthread_attr_t attr;

    int ret = pthread_attr_init(&attr);
    if(ret != 0)
    {
	sys_err("pth_attr_init error", ret);
    }

    int ret1 = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(ret1 != 0)
    {
	sys_err("ptn_attr_setdetachstate error", ret1);
    }

    int ret2 = pthread_create(&tid, &attr, pthread_fun, NULL);
    if(ret2 != 0)
    {
	sys_err("pth_create error", ret2);
    }
    
    int ret3 = pthread_join(tid, NULL);
    if(ret3 != 0)
    {
	sys_err("pth_join error", ret3);
    }
    
    pthread_attr_destroy(&attr);
    return 0;
}

