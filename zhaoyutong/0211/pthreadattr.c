#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void sys_err(char* str,int nu)
{
  fprintf(stderr,"%s:%s\n",str,strerror(nu));
  pthread_exit(NULL);
}
void* pthread_fun(void* arg)
{
  printf("child pthread\n");
  return NULL;
}
int main(int argc, char* argv[])
  {
    pthread_t tid;
    pthread_attr_t attr;
    int ret1= pthread_attr_init(&attr);
    if(ret1!=0)
    {
     sys_err("pthread_attr_init error",ret1);
    }
    int reat2= pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
   if(reat2!=0)
    {
     sys_err("pthread_attr_setdetachstate error",reat2);
    }
    pthread_create(&tid,&attr,pthread_fun,NULL);
    // pthread_detach(tid);
    int pjr= pthread_join(tid,NULL);
    if(pjr!=0)
    {
     sys_err("pthread_join error",pjr);
    }
    pthread_exit(NULL);
  }


