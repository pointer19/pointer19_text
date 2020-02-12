#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

void sys_err(const char* str,int errno)
{
//    strerror(errno);//fan yi  cuo wu hao
    fprintf(stderr, "%s: %s", str, strerror(errno));
    exit(1);
}

void* pthread_fun(void * arg)//xian  cheng  zhu  han  shu  
{//zi  xian  cheng
    int i = (int)arg;
    printf("i = %d,pid = %d, tid = %lu\n", i, getpid(), pthread_self());
    return NULL;
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
    pthread_t tid1;
    int i;
    for(i = 0; i < 5; i++)
    {
//	sleep(1);
	pthread_create(&tid1, NULL, pthread_fun, (void *)i);
	printf("%d\n",i);
    }
    sleep(1);

    return 0;//exit(0);jieshujincheng  shifangkongjian
}

