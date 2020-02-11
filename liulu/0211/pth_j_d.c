#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void sys_err(char* str, int nu)
{
    fprintf(stderr, "%s:%s\n", str, strerror(nu));
    exit(1);
}

void* pthread_fun(void* arg)
{
    printf("chile pth\n");
    return NULL;
}
int main(int argc, char* argv[])
{ 
    pthread_t tid;
    pthread_attr_t attr;
    int ret = pthread_attr_init(&attr);
    if(ret != 0)
    {
	sys_err("pth_attr_init", ret);
    }
    int ret1 = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(ret1 != 0)
    {
		sys_err("pth_attr_setdetachstate", ret1);
    }
    int ret2 = pthread_create(&tid, &attr, pthread_fun, NULL);
    if(ret2 != 0)
    {
		sys_err("creat error", ret2);
    }

    int pj = pthread_join(tid, NULL);
    if(pj != 0)
    {
	fprintf(stderr,"join error:%s\n",strerror(pj));
	exit(1);
    }

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
}
