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
   
   
    printf("pid = %d, tid = %lu\n", getpid(), pthread_self());
    exit(0);
    //   return NULL;
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, pthread_fun, NULL);


    sleep(1);
    printf("sssssssssssssss");
    return 0;//exit(0);jieshujincheng  shifangkongjian
}

