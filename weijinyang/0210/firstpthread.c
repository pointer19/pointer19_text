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
    printf("pid = %d, tid = %lu\n",getpid(), pthread_self());
    return NULL;
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    pthread_t tid1;
    int retval = pthread_create(&tid1, NULL, pthread_fun, NULL);
    if(retval != 0)
    {
	sys_err("pthread_create error", retval);
    }
    printf("pid = %d, ztid = %lu\n",pid, tid);
    sleep(1); //bushui jincheng  jieshu  le 

    return 0;//exit(0);jieshujincheng  shifangkongjian
}

