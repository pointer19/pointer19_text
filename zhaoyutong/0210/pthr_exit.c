#include<stdio.h>
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
void fun1()
{
   pthread_exit(NULL);  
}
void* pthread_fun(void * arg)
{   
    // return NULL;
     printf("++++++++++\n");
     fun1();
    // pthread_exit(NULL);
     printf("----------------\n");
}
int main(int argc, char* argv[])
  {
    pthread_t tid;
    pthread_create(&tid,NULL,pthread_fun,NULL);
    printf("////////\n");
    pthread_exit(NULL);
    sleep(1);
    return 0;
  }

