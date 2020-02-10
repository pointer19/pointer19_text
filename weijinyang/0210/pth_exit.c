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

void* fun1()
{
    pthread_exit(NULL);
}

void* pthread_fun(void * arg)//xian  cheng  zhu  han  shu  
{//zi  xian  cheng
   
    sleep(2);
    fun1();
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    //return NULL;

    pthread_exit(NULL);
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, pthread_fun, NULL);
    
    pthread_exit(NULL);
   // sleep(1);
   // printf("sssssssssssssss");
   // return 0;//exit(0);jieshujincheng  shifangkongjian
}

