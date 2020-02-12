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
    while(1)
    {//dei  jin  ru nei  he cai  neng sha  si ,li mian  sha ye mei you sha bu si
//	printf("sssssssssss\n");
//	sleep(1);
	pthread_testcancel();
    }
    return NULL;
}
int main(int argc, char* argv[])//zhu  xian  cheng
{
   
    
    pthread_t tid1;
    int retval = pthread_create(&tid1, NULL, pthread_fun, NULL);
    if(retval != 0)
    {
	sys_err("pthread_create error", retval);
    }
  
    sleep(3); //bushui jincheng  jieshu  le 
    pthread_cancel(tid1);
    return 0;//exit(0);jieshujincheng  shifangkongjian
}

