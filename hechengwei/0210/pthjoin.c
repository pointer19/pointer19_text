#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<pthread.h>
#define STR "123456\n"
void sys_err(char *str,int ernu)
{
fprintf(stderr,"%s:%s",str,strerror(ernu));
exit(1);

}
void * pthread_fun(void *arg)
{
sleep(3);
printf("pid =%d,tid=%lu\n",getpid(),pthread_self());
return (void*)100;
}
int main(int argc,char * argv[])
{
int *retval;
pthread_t tid;
pthread_create(&tid,NULL,pthread_fun,NULL);
pthread_join(tid,(void **)&retval);

printf("retval =%d\n",(int )retval);
pthread_exit(NULL);
}