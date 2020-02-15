#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
void sys_err(char *str,int ernu)
{   fprintf(stderr,"%s: %s",str,strerror(ernu));
    exit(1);
}
void* pthread_fun(void * arg)
{
     printf("pid=%d,tid=%lu\n",getpid(),pthread_self());
}
int main(int argc, char* argv[])
  {
    pid_t pid=getpid();
    pthread_t tid1=pthread_self();
    pthread_t tid;
    pthread_create(&tid,NULL,pthread_fun,NULL);
    printf("pid=%d,tid1=%lu\n",pid,tid1);
    sleep(1);
    return 0;
  }

